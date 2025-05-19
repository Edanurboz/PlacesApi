import UIKit
import GoogleMaps
import GooglePlaces
import CoreLocation

class ViewController: UIViewController, CLLocationManagerDelegate, UISearchBarDelegate, GMSMapViewDelegate{

    @IBOutlet weak var mapView: GMSMapView!
    @IBOutlet weak var searchBar: UISearchBar!

    let locationManager = CLLocationManager()
    var currentCoordinate: CLLocationCoordinate2D?

    override func viewDidLoad() {
        super.viewDidLoad()

            locationManager.delegate = self
            searchBar.delegate = self
            mapView.delegate = self 

            locationManager.requestWhenInUseAuthorization()
            locationManager.startUpdatingLocation()

            mapView.isMyLocationEnabled = true
    }

   
    func locationManager(_ manager: CLLocationManager, didUpdateLocations locations: [CLLocation]) {
        guard let location = locations.first else { return }
        currentCoordinate = location.coordinate

        let camera = GMSCameraPosition.camera(withLatitude: location.coordinate.latitude,
                                              longitude: location.coordinate.longitude,
                                              zoom: 15)
        mapView.animate(to: camera)

        
        let marker = GMSMarker()
        marker.position = location.coordinate
        marker.title = "Ben"
        marker.map = mapView

        locationManager.stopUpdatingLocation()
    }

    
    func searchBarSearchButtonClicked(_ searchBar: UISearchBar) {
        searchBar.resignFirstResponder() 

        guard let keyword = searchBar.text?.lowercased(), !keyword.isEmpty else { return }
        guard let coordinate = currentCoordinate else { return }

        
        mapView.clear()

        
        let selfMarker = GMSMarker()
        selfMarker.position = coordinate
        selfMarker.title = "Ben"
        selfMarker.map = mapView

        fetchNearbyPlaces(location: coordinate, keyword: keyword)
    }

    
    func fetchNearbyPlaces(location: CLLocationCoordinate2D, keyword: String) {
        let radius = 1000 
        let apiKey = "AIzaSyD-E5q6andp6BmSQq4h0syiDTVZGtovsQ4"
        let urlStr = "https://maps.googleapis.com/maps/api/place/nearbysearch/json?location=\(location.latitude),\(location.longitude)&radius=\(radius)&keyword=\(keyword)&key=\(apiKey)"

        guard let url = URL(string: urlStr) else { return }

        URLSession.shared.dataTask(with: url) { data, _, error in
            guard let data = data, error == nil else { return }

            do {
                if let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
                   let results = json["results"] as? [[String: Any]] {

                    DispatchQueue.main.async {
                        for place in results {
                            if let geometry = place["geometry"] as? [String: Any],
                               let loc = geometry["location"] as? [String: Double],
                               let name = place["name"] as? String {

                                let marker = GMSMarker()
                                marker.position = CLLocationCoordinate2D(latitude: loc["lat"]!, longitude: loc["lng"]!)
                                marker.title = name
                                marker.userData = place 
                                marker.map = self.mapView
                            }
                        }
                    }
                }
            } catch {
                print("JSON parse error: \(error.localizedDescription)")
            }
        }.resume()
    }
    
    func mapView(_ mapView: GMSMapView, didTap marker: GMSMarker) -> Bool {
        if let placeData = marker.userData as? [String: Any],
           let detailVC = storyboard?.instantiateViewController(withIdentifier: "PlaceDetailViewController") as? PlaceDetailViewController {

            
            detailVC.placeName = placeData["name"] as? String

            
            if let geometry = placeData["geometry"] as? [String: Any],
               let location = geometry["location"] as? [String: Double],
               let lat = location["lat"],
               let lng = location["lng"] {
                detailVC.placeCoordinate = CLLocationCoordinate2D(latitude: lat, longitude: lng)
            }

            
            detailVC.userLocation = currentCoordinate

            
            self.present(detailVC, animated: true, completion: nil)
        }
        return true
    }

}
