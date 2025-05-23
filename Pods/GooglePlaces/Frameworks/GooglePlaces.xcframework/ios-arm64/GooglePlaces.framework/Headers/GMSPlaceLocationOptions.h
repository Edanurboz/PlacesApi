//
//  GMSPlaceLocationOptions.h
//  Google Places SDK for iOS
//
//  Copyright 2019 Google LLC
//
//  Usage of this SDK is subject to the Google Maps/Google Earth APIs Terms of
//  Service: https://cloud.google.com/maps-platform/terms
//


#import <CoreLocation/CoreLocation.h>


NS_ASSUME_NONNULL_BEGIN

#pragma mark - Protocols

/** Protocol for specifying that the location can be used as search bias. */
@protocol GMSPlaceLocationBias <NSObject, NSCopying>

/** Returns the location bias as `NSURLQueryItem`. */
- (NSURLQueryItem *)locationBiasURLQueryItem;

@end

/** Protocol for specifying that the location can be used as search restriction. */
@protocol GMSPlaceLocationRestriction <NSObject, NSCopying>

/** Returns the location restriction as `NSURLQueryItem`. */
- (NSURLQueryItem *)locationRestrictionURLQueryItem;

@end

/**
 * \defgroup PlaceRectangularLocationOption GMSPlaceRectangularLocationOption
 * @{
 */

/**
 * Returns a rectangular location to filter place results inside the boundaries.
 * Supports filtering as a restriction where results must be inside the bounds, or as a bias where
 * results in the bounds are preferred.
 *
 * @param northEastBounds The north east corner of the bounds.
 * @param southWestBounds The south west corner of the bounds.
 */
FOUNDATION_EXTERN id<GMSPlaceLocationBias, GMSPlaceLocationRestriction>
GMSPlaceRectangularLocationOption(CLLocationCoordinate2D northEastBounds,
                                  CLLocationCoordinate2D southWestBounds);
/**@}*/

/**
 * \defgroup PlaceCircularLocationOption GMSPlaceCircularLocationOption
 * @{
 */

/**
 * Returns a circular location to bias place results.
 * Supports filtering as a bias where results inside the circle are preferred.
 *
 * @param center The center of the circle.
 * @param radius The radius of the circle.
 */
FOUNDATION_EXTERN id<GMSPlaceLocationBias, GMSPlaceLocationRestriction>
GMSPlaceCircularLocationOption(CLLocationCoordinate2D center, CLLocationDistance radius);
/**@}*/

NS_ASSUME_NONNULL_END
