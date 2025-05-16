import UIKit
import CoreLocation

class PlaceDetailViewController: UIViewController {

    var placeName: String?
    var placeCoordinate: CLLocationCoordinate2D?
    var userLocation: CLLocationCoordinate2D?

    @IBOutlet weak var nameLabel: UILabel!
    @IBOutlet weak var distanceLabel: UILabel!

    override func viewDidLoad() {
        super.viewDidLoad()
       

        if let name = placeName {
                   nameLabel.text = "Adı: \(name)"
               }

        if let user = userLocation, let place = placeCoordinate {
            let userLoc = CLLocation(latitude: user.latitude, longitude: user.longitude)
            let placeLoc = CLLocation(latitude: place.latitude, longitude: place.longitude)

            let distanceInMeters = userLoc.distance(from: placeLoc)
            let distanceInKM = distanceInMeters / 1000

            distanceLabel.text = String(format: "Mesafe: %.2f km", distanceInKM)
        }
    }
}
