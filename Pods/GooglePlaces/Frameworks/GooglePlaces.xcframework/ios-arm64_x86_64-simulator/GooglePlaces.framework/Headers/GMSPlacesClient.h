//
//  GMSPlacesClient.h
//  Google Places SDK for iOS
//
//  Copyright 2016 Google LLC
//
//  Usage of this SDK is subject to the Google Maps/Google Earth APIs Terms of
//  Service: https://cloud.google.com/maps-platform/terms
//

#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>

#import "GMSPlace.h"
#import "GMSPlaceFieldMask.h"
#import "GMSPlacesDeprecationUtils.h"
#import "GMSPlacesErrors.h"

@class GMSAutocompleteFilter;
@class GMSAutocompletePrediction;
@class GMSAutocompleteSessionToken;
@class GMSAutocompleteRequest;
@class GMSAutocompleteSuggestion;
@class GMSPlace;
@class GMSPlaceLikelihood;
@class GMSPlaceLikelihoodList;
@class GMSPlacePhotoMetadata;
@class GMSPlacePhotoMetadataList;
@class GMSPlaceSearchByTextRequest;
@class GMSFetchPlaceRequest;
@class GMSFetchPhotoRequest;
@class GMSPlaceSearchNearbyRequest;

@class GMSPlaceIsOpenRequest;
@class GMSPlaceIsOpenResponse;

@protocol GMSPlacesAppCheckTokenProvider;

NS_ASSUME_NONNULL_BEGIN

/**
 * Callback type for receiving place details lookups. If an error occurred,
 * `result` will be nil and `error` will contain information about the error.
 * @param result The `GMSPlace` that was returned.
 * @param error The error that occurred, if any.
 *
 * @see `GMSPlacesClient`
 */
typedef void (^GMSPlaceResultCallback)(GMSPlace *_Nullable result, NSError *_Nullable error);

/**
 * Callback type for receiving place likelihood lists. If an error occurred, `likelihoodList` will
 * be nil and `error` will contain information about the error.
 * @param likelihoodList The list of place likelihoods.
 * @param error The error that occurred, if any.
 *
 * @see `GMSPlacesClient`
 */
typedef void (^GMSPlaceLikelihoodListCallback)(GMSPlaceLikelihoodList *_Nullable likelihoodList,
                                               NSError *_Nullable error);

/**
 * Callback type for receiving array of `GMSPlaceLikelihood`s. If an error occurred, the array will
 * be nil and `error` will contain information about the error.
 *
 * @see `GMSPlacesClient`
 */
typedef void (^GMSPlaceLikelihoodsCallback)(NSArray<GMSPlaceLikelihood *> *_Nullable likelihoods,
                                            NSError *_Nullable error);

/**
 * Callback type for receiving autocompletion results. `results` is an array of
 * `GMSAutocompletePredictions` representing candidate completions of the query.
 * @param results An array of `GMSAutocompletePrediction`s.
 * @param error The error that occurred, if any.
 *
 * @see `GMSPlacesClient`
 */
typedef void (^GMSAutocompletePredictionsCallback)(
    NSArray<GMSAutocompletePrediction *> *_Nullable results, NSError *_Nullable error);

/**
 * Callback type for receiving place photos results. If an error occurred, `photos` will be nil and
 * `error` will contain information about the error.
 * @param photos The result containing `GMSPlacePhotoMetadata` objects.
 * @param error The error that occurred, if any.
 *
 * @see `GMSPlacesClient`
 */
typedef void (^GMSPlacePhotoMetadataResultCallback)(GMSPlacePhotoMetadataList *_Nullable photos,
                                                    NSError *_Nullable error);

/**
 * Callback type for receiving `UIImage` objects from a `GMSPlacePhotoMetadata` object. If an error
 * occurred, `photo` will be nil and `error` will contain information about the error.
 * @param photo The `UIImage` which was loaded.
 * @param error The error that occurred, if any.
 *
 * @see `GMSPlacesClient`
 */
typedef void (^GMSPlacePhotoImageResultCallback)(UIImage *_Nullable photo,
                                                 NSError *_Nullable error);

/**
 * Callback type for receiving the opening hours status for the Place. If an error occurred,
 * `result` will be `GMSPlaceOpenStatusUnknown` and `error` will contain information about the
 * error.
 * @param result The `GMSPlaceOpenStatus` that was returned.
 * @param error The error that occurred, if any.
 *
 * @see `GMSPlacesClient`
 */
typedef void (^GMSPlaceOpenStatusCallback)(GMSPlaceOpenStatus result, NSError *_Nullable error);

/**
 * Callback type for receiving the open status response. If an error occurred, response will be
 * have a status of `GMSPlaceOpenStatusUnknown` and error will contain information about the error.
 * @param response The `GMSPlaceIsOpenResponse` that was returned.
 * @param error The error that occurred, if any.
 *
 * @see `GMSPlacesClient`
 */
typedef void (^GMSPlaceOpenStatusResponseCallback)(GMSPlaceIsOpenResponse *response,
                                                   NSError *_Nullable error);

/**
 * Callback type for receiving search by text results. `results` is an array of
 * `GMSPlace` representing individual results matching the query.
 * @param results An array of `GMSPlace`s.
 * @param error The error that occurred, if any.
 *
 * @see `GMSPlacesClient`
 */

typedef void (^GMSPlaceSearchByTextResultCallback)(NSArray<GMSPlace *> *_Nullable places,
                                                   NSError *_Nullable error);

/**
 * Callback type for receiving a photo. `photoImage` is a `UIImage`
 * representing the resulting photo matching the specified request.
 * If an error occurred, `photoImage` will be nil and `error` will contain
 * information about the error.
 * @param photoImage A `UIImage` result.
 *
 * @see `GMSPlacesClient`
 */

typedef void (^GMSFetchPhotoResultCallback)(UIImage *_Nullable photoImage,
                                            NSError *_Nullable error);

/**
 * Callback type for autocomplete results.
 * @param results An array of `GMSAutocompleteSuggestion`.
 * @param error The error that occurred, if any.
 *
 * @see `GMSPlacesClient`
 */
typedef void (^GMSAutocompleteSuggestionsCallback)(
    NSArray<GMSAutocompleteSuggestion *> *_Nullable results, NSError *_Nullable error);

/**
 * Callback type for receiving search nearby results.
 * @param places An array of `GMSPlace`
 * @param error The error that occurred, if any.
 *
 * @see `GMSPlacesClient`
 */
typedef void (^GMSPlaceSearchNearbyResultCallback)(NSArray<GMSPlace *> *_Nullable places,
                                                   NSError *_Nullable error);

/**
 * Main interface to the Places SDK. Used for searching and getting details about places. This class
 * should be accessed through the `[GMSPlacesClient sharedClient]` method.
 *
 * `GMSPlacesClient` methods should only be called from the main thread. Calling these methods from
 * another thread will result in an exception or undefined behavior. Unless otherwise specified, all
 * callbacks will be invoked on the main thread.
 */
@interface GMSPlacesClient : NSObject

/**
 * Provides the shared instance of `GMSPlacesClient` for the Google Places SDK for iOS, creating it
 * if necessary.
 *
 * If your application often uses methods of `GMSPlacesClient` it may want to hold onto this object
 * directly, as otherwise your connection to Google may be restarted on a regular basis.
 */
+ (instancetype)sharedClient;

/**
 * Provides your API key to the Google Places SDK for iOS. This key is generated for your
 * application via the Google Cloud Platform Console, and is paired with your application's
 * bundle ID to identify it. This should be called by your application before using
 * `GMSPlacesClient` (e.g., in `application:didFinishLaunchingWithOptions:`).
 *
 * @return YES if the APIKey was successfully provided.
 */
+ (BOOL)provideAPIKey:(NSString *)key;

/**
 * Provides an App Check token provider to the Google Places SDK for iOS.  This should be called by
 * your application before using `GMSPlacesClient`
 * (for example, in `application:didFinishLaunchingWithOptions:`). If you do not provide a token
 * provider, the SDK will not use the token provider.
 */
+ (void)setAppCheckTokenProvider:(id<GMSPlacesAppCheckTokenProvider>)provider;

/**
 * Returns the open source software license information for the Google Places SDK for iOS. This
 * information must be made available within your application.
 */
+ (NSString *)openSourceLicenseInfo;

/** Returns the version for this release of the Google Places SDK for iOS.. For example, "1.0.0". */
+ (NSString *)SDKVersion;

/**
 * Returns the long version for this release of the Google Places SDK for iOS.. For example, "1.0.0
 * (102.1)".
 */
+ (NSString *)SDKLongVersion;

/**
 * Get details for a place. This method is non-blocking.
 * @param placeID The place ID to lookup.
 * @param callback The callback to invoke with the lookup result.
 */
- (void)lookUpPlaceID:(NSString *)placeID
             callback:(GMSPlaceResultCallback)callback __GMS_AVAILABLE_BUT_DEPRECATED_MSG(
                          "This method is replaced by <code>fetchPlaceWithRequest:callback:</code> "
                          "and will be removed in a future release.");

/**
 * Find Autocomplete suggestions from text query. Results may optionally be biased towards a
 * certain location or restricted to an area. This method is non-blocking.
 *
 * The supplied callback will be invoked with an array of autocompletion suggestions upon success
 * and an `NSError` upon an error.
 *
 * @param request The `GMSAutocompleteRequest` request for autocomplete.
 * @param callback The callback to invoke with the suggestions.
 */
- (void)fetchAutocompleteSuggestionsFromRequest:(GMSAutocompleteRequest *)request
                                       callback:(GMSAutocompleteSuggestionsCallback)callback;

/**
 * Gets the metadata for up to 10 photos associated with a place.
 *
 * Photos are sourced from a variety of locations, including business owners and photos contributed
 * by Google+ users. In most cases, these photos can be used without attribution, or will have the
 * required attribution included as a part of the image. However, you must use the `attributions`
 * property in the response to retrieve any additional attributions required, and display those
 * attributions in your application wherever you display the image. A maximum of 10 photos are
 * returned.
 *
 * Multiple calls of this method will probably return the same photos each time. However, this is
 * not guaranteed because the underlying data may have changed.
 *
 * This method performs a network lookup.
 *
 * @param placeID The place ID for which to lookup photos.
 * @param callback The callback to invoke with the lookup result.
 */
- (void)lookUpPhotosForPlaceID:(NSString *)placeID
                      callback:(GMSPlacePhotoMetadataResultCallback)callback;

/**
 * Loads the image for a specific photo at its maximum size.
 *
 * Image data may be cached by the SDK. If the requested photo does not exist in the cache then a
 * network lookup will be performed.
 *
 * @param photoMetadata The `GMSPlacePhotoMetadata` for which to load a `UIImage`.
 * @param callback The callback to invoke with the loaded `UIImage`.
 */
- (void)loadPlacePhoto:(GMSPlacePhotoMetadata *)photoMetadata
              callback:(GMSPlacePhotoImageResultCallback)callback;

/**
 * Loads the image for a specific photo, scaled to fit the given maximum dimensions.
 *
 * The image will be scaled to fit within the given dimensions while maintaining the aspect ratio of
 * the original image. This scaling is performed server-side.
 *
 * If the scale parameter is not 1.0 maxSize will be multiplied by this value and the returned
 * `UIImage` will be set to have the specified scale. This parameter should be set to the screen
 * scale if you are loading images for display on screen.
 *
 * Image data may be cached by the SDK. If the requested photo does not exist in the cache then a
 * network lookup will be performed.
 *
 * NOTE: After applying the scale factor the dimensions in maxSize will be rounded up to the nearest
 * integer before use. If an image is requested which is larger than the maximum size available a
 * smaller image may be returned.
 *
 * @param photoMetadata The `GMSPlacePhotoMetadata` for which to load a `UIImage`.
 * @param maxSize The maximum size of the image.
 * @param scale The scale to load the image at.
 * @param callback The callback to invoke with the loaded `UIImage`.
 */
- (void)loadPlacePhoto:(GMSPlacePhotoMetadata *)photoMetadata
     constrainedToSize:(CGSize)maxSize
                 scale:(CGFloat)scale
              callback:(GMSPlacePhotoImageResultCallback)callback;

/**
 * Returns an estimate of the place where the device is currently known to be located.
 *
 * Generates a place likelihood list based on the device's last estimated location. The supplied
 * callback will be invoked with this likelihood list upon success and an NSError upon an error.
 *
 * NOTE: This method requires that your app has permission to access the current device location.
 * Before calling this make sure to request access to the users location using [CLLocationManager
 * requestWhenInUseAuthorization] or [CLLocationManager requestAlwaysAuthorization]. If you do call
 * this method and your app does not have the correct authorization status, the callback will be
 * called with an error.
 *
 * @param callback The callback to invoke with the place likelihood list.
 */
- (void)currentPlaceWithCallback:(GMSPlaceLikelihoodListCallback)callback;

/**
 * Find Autocomplete predictions from text query. Results may optionally be biased towards a
 * certain location or restricted to an area. This method is non-blocking.
 *
 * The supplied callback will be invoked with an array of autocompletion predictions upon success
 * and an NSError upon an error.
 *
 * @param query The partial text to autocomplete.
 * @param filter The filter to apply to the results. This parameter may be nil.
 * @param sessionToken The `GMSAutocompleteSessionToken` to associate request to a billing session.
 * @param callback The callback to invoke with the predictions.
 */
- (void)findAutocompletePredictionsFromQuery:(NSString *)query
                                      filter:(nullable GMSAutocompleteFilter *)filter
                                sessionToken:(nullable GMSAutocompleteSessionToken *)sessionToken
                                    callback:(GMSAutocompletePredictionsCallback)callback;

/**
 * Fetch details for a place. This method is non-blocking.
 * @param placeID The place ID to lookup.
 * @param placeFields The individual place fields requested for the place objects in the list.
 * @param sessionToken The `GMSAutocompleteSessionToken` to associate request to a billing session.
 * @param callback The callback to invoke with the lookup result.
 */
- (void)fetchPlaceFromPlaceID:(NSString *)placeID
                  placeFields:(GMSPlaceField)placeFields
                 sessionToken:(nullable GMSAutocompleteSessionToken *)sessionToken
                     callback:(GMSPlaceResultCallback)callback __GMS_AVAILABLE_BUT_DEPRECATED_MSG(
                                  "This method is replaced by <code>fetchPlaceWithRequest:callback:"
                                  "callback:</code> and will be removed in a future release.");

/**
 * Find place likelihoods using the user's current location. This method is non-blocking.
 *
 * The supplied callback will be invoked with an array of places with likelihood scores upon success
 * and an NSError upon an error.
 *
 * @param placeFields The individual place fields requested for the place objects in the list.
 * @param callback The callback to invoke with place likelihoods.
 */
- (void)findPlaceLikelihoodsFromCurrentLocationWithPlaceFields:(GMSPlaceField)placeFields
                                                      callback:
                                                          (GMSPlaceLikelihoodsCallback)callback;

/**
 * Gets the open status for a place. Gets details for a place including all properties necessary to
 * determine `GMSPlaceOpenStatus` at the specified `NSDate`. This method is non-blocking.
 * @param isOpenRequest The request to determine the open status for a given place.
 * @param callback The callback to invoke with the open status response.
 */
- (void)isOpenWithRequest:(GMSPlaceIsOpenRequest *)isOpenRequest
                 callback:(GMSPlaceOpenStatusResponseCallback)callback;

/**
 * Gets details for a place including all fields necessary to determine `GMSPlaceOpenStatus` at the
 * current time. This method is non-blocking.
 * @param placeID The place ID to lookup.
 * @param callback The callback to invoke with the place result.
 */
- (void)isOpenWithPlaceID:(NSString *)placeID callback:(GMSPlaceOpenStatusCallback)callback;

/**
 * Gets details for a place including all fields necessary to determine `GMSPlaceOpenStatus` at the
 * specified `NSDate`. This method is non-blocking.
 * @param placeID The place ID to lookup.
 * @param date The `NSDate` to determine open status for.
 * @param callback The callback to invoke with the place result.
 */
- (void)isOpenWithPlaceID:(NSString *)placeID
                     date:(NSDate *)date
                 callback:(GMSPlaceOpenStatusCallback)callback;

/**
 * Gets details for a place including all fields necessary to determine `GMSPlaceOpenStatus` at the
 * current time. Only requests additional fields if the `GMSPlace` does not have all necessary
 * fields, otherwise `GMSPlaceOpenStatus` will be returned in the callback immediately. This method
 * is non-blocking.
 * @param place The `GMSPlace` to lookup.
 * @param callback The callback to invoke with the place result.
 */
- (void)isOpenWithPlace:(GMSPlace *)place callback:(GMSPlaceOpenStatusCallback)callback;

/**
 * Gets details for a place including all fields necessary to determine `GMSPlaceOpenStatus` at the
 * specified `NSDate`. Only requests additional fields if the `GMSPlace` does not have all necessary
 * fields, otherwise `GMSPlaceOpenStatus` will be returned in the callback immediately. This method
 * is non-blocking.
 * @param place The `GMSPlace` to lookup.
 * @param date The `NSDate` to determine open status for.
 * @param callback The callback to invoke with the place result.
 */
- (void)isOpenWithPlace:(GMSPlace *)place
                   date:(NSDate *)date
               callback:(GMSPlaceOpenStatusCallback)callback;

/**
 * Search for places by text and restrictions. This method is non-blocking.
 * @param textSearchRequest `GMSPlaceSearchByTextRequest` The text request to use for the query.
 * @param callback The callback to invoke with the lookup result.
 */

- (void)searchByTextWithRequest:(GMSPlaceSearchByTextRequest *)textSearchRequest
                       callback:(GMSPlaceSearchByTextResultCallback)callback;

/**
 * Get a place using a request object. This method is non-blocking.
 * @param fetchPlaceRequest `GMSFetchPlaceRequest` The fetch place request to use for the query.
 * @param callback The callback to invoke with the place result.
 */

- (void)fetchPlaceWithRequest:(GMSFetchPlaceRequest *)fetchPlaceRequest
                     callback:(GMSPlaceResultCallback)callback;

/**
 * Request a photo using fetch photo request. This method is non-blocking.
 * @param fetchPhotoRequest `GMSFetchPhotoRequest` The photo request to use.
 * @param callback The callback to invoke with the `NSURL` result.
 */

- (void)fetchPhotoWithRequest:(GMSFetchPhotoRequest *)fetchPhotoRequest
                     callback:(GMSFetchPhotoResultCallback)callback;


/**
 * Search for places near a location and restriction. This method is non-blocking.
 * @param searchNearbyRequest `GMSPlaceSearchNearbyRequest` The search nearby request to use for
 * the query.
 * @param callback The callback to invoke with the lookup result.
 */
- (void)searchNearbyWithRequest:(GMSPlaceSearchNearbyRequest *)searchNearbyRequest
                       callback:(GMSPlaceSearchNearbyResultCallback)callback;

/**
 * Adds a usage attribution ID to the initializer, which helps Google understand which libraries and
 * samples are helpful to developers, such as usage of a marker clustering library.
 * To opt out of sending the usage attribution ID, it is safe to delete this function call or
 * replace the value with an empty string.
 *
 * @param internalUsageAttributionId The usage attribution ID to add
 */
+ (void)addInternalUsageAttributionID:(NSString *)internalUsageAttributionID;

@end

NS_ASSUME_NONNULL_END
