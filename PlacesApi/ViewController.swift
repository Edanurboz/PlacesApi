import UIKit
import GoogleMaps
import GooglePlaces
import CoreLocation

class ViewController: UIViewController, CLLocationManagerDelegate, UISearchBarDelegate {

    @IBOutlet weak var mapView: GMSMapView!
    @IBOutlet weak var searchBar: UISearchBar!

    let locationManager = CLLocationManager()
    var currentCoordinate: CLLocationCoordinate2D?

    override func viewDidLoad() {
        super.viewDidLoad()

        // Delegeleri ata
        locationManager.delegate = self
        searchBar.delegate = self

        // Konum yetkisi ve başlat
        locationManager.requestWhenInUseAuthorization()
        locationManager.startUpdatingLocation()

        mapView.isMyLocationEnabled = true
    }

    // Konum güncellendiğinde
    func locationManager(_ manager: CLLocationManager, didUpdateLocations locations: [CLLocation]) {
        guard let location = locations.first else { return }
        currentCoordinate = location.coordinate

        let camera = GMSCameraPosition.camera(withLatitude: location.coordinate.latitude,
                                              longitude: location.coordinate.longitude,
                                              zoom: 15)
        mapView.animate(to: camera)

        // Kullanıcıya marker
        let marker = GMSMarker()
        marker.position = location.coordinate
        marker.title = "Ben"
        marker.map = mapView

        locationManager.stopUpdatingLocation()
    }

    // Arama yapıldığında
    func searchBarSearchButtonClicked(_ searchBar: UISearchBar) {
        searchBar.resignFirstResponder() // Klavyeyi kapat

        guard let keyword = searchBar.text?.lowercased(), !keyword.isEmpty else { return }
        guard let coordinate = currentCoordinate else { return }

        // Önce haritadaki eski markerları temizle
        mapView.clear()

        // Kendi konumunu yeniden ekle
        let selfMarker = GMSMarker()
        selfMarker.position = coordinate
        selfMarker.title = "Ben"
        selfMarker.map = mapView

        fetchNearbyPlaces(location: coordinate, keyword: keyword)
    }

    // Google Places API isteği
    func fetchNearbyPlaces(location: CLLocationCoordinate2D, keyword: String) {
        let radius = 1000 // metre
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
}
