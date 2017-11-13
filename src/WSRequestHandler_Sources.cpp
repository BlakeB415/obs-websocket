#include <QString>
#include "Utils.h"

#include "WSRequestHandler.h"

/**
 * Show or hide a specified source item in a specified scene.
 * 
 * @param {String} `source` Name of the source in the specified scene.
 * @param {boolean} `render` Desired visibility.
 * @param {String (optional)} `scene-name` Name of the scene where the source resides. Defaults to the currently active scene.
 *
 * @api requests
 * @name SetSourceRender
 * @category sources
 * @since 0.3
 * @deprecated Since unreleased. Prefer the use of SetSceneItemProperties.
 */
 void WSRequestHandler::HandleSetSceneItemRender(WSRequestHandler* req) {
    if (!req->hasField("source") ||
        !req->hasField("render"))
    {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    const char* itemName = obs_data_get_string(req->data, "source");
    bool isVisible = obs_data_get_bool(req->data, "render");

    if (!itemName) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    const char* sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene doesn't exist");
        return;
    }

    OBSSceneItemAutoRelease sceneItem =
        Utils::GetSceneItemFromName(scene, itemName);
    if (sceneItem) {
        obs_sceneitem_set_visible(sceneItem, isVisible);
        req->SendOKResponse();
    } else {
        req->SendErrorResponse("specified scene item doesn't exist");
    }
}

/**
 * Set the volume of the specified source.
 *
 * @param {String} `source` Name of the source.
 * @param {double} `volume` Desired volume. Must be between `0.0` and `1.0`.
 *
 * @api requests
 * @name SetVolume
 * @category sources
 * @since 4.0.0
 */
 void WSRequestHandler::HandleSetVolume(WSRequestHandler* req) {
    if (!req->hasField("source") ||
        !req->hasField("volume"))
    {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    QString sourceName = obs_data_get_string(req->data, "source");
    float sourceVolume = obs_data_get_double(req->data, "volume");

    if (sourceName.isEmpty() ||
        sourceVolume < 0.0 || sourceVolume > 1.0) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    OBSSourceAutoRelease source = obs_get_source_by_name(sourceName.toUtf8());
    if (!source) {
        req->SendErrorResponse("specified source doesn't exist");
        return;
    }

    obs_source_set_volume(source, sourceVolume);
    req->SendOKResponse();
}

/**
 * Get the volume of the specified source.
 *
 * @param {String} `source` Name of the source.
 *
 * @return {String} `name` Name of the source.
 * @return {double} `volume` Volume of the source. Between `0.0` and `1.0`.
 * @return {boolean} `mute` Indicates whether the source is muted.
 *
 * @api requests
 * @name GetVolume
 * @category sources
 * @since 4.0.0
 */
void WSRequestHandler::HandleGetVolume(WSRequestHandler* req) {
    if (!req->hasField("source")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    QString sourceName = obs_data_get_string(req->data, "source");
    if (!sourceName.isEmpty()) {
        OBSSourceAutoRelease source = obs_get_source_by_name(sourceName.toUtf8());

        OBSDataAutoRelease response = obs_data_create();
        obs_data_set_string(response, "name", sourceName.toUtf8());
        obs_data_set_double(response, "volume", obs_source_get_volume(source));
        obs_data_set_bool(response, "muted", obs_source_muted(source));

        req->SendOKResponse(response);
    } else {
        req->SendErrorResponse("invalid request parameters");
    }
}

/**
 * Inverts the mute status of a specified source.
 *
 * @param {String} `source` The name of the source.
 *
 * @api requests
 * @name ToggleMute
 * @category sources
 * @since 4.0.0
 */
void WSRequestHandler::HandleToggleMute(WSRequestHandler* req) {
    if (!req->hasField("source")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    QString sourceName = obs_data_get_string(req->data, "source");
    if (sourceName.isEmpty()) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    OBSSourceAutoRelease source = obs_get_source_by_name(sourceName.toUtf8());
    if (!source) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    obs_source_set_muted(source, !obs_source_muted(source));
    req->SendOKResponse();
}

/**
 * Sets the mute status of a specified source.
 *
 * @param {String} `source` The name of the source.
 * @param {boolean} `mute` Desired mute status.
 *
 * @api requests
 * @name SetMute
 * @category sources
 * @since 4.0.0
 */
void WSRequestHandler::HandleSetMute(WSRequestHandler* req) {
    if (!req->hasField("source") ||
        !req->hasField("mute")) {
        req->SendErrorResponse("mssing request parameters");
        return;
    }

    QString sourceName = obs_data_get_string(req->data, "source");
    bool mute = obs_data_get_bool(req->data, "mute");

    if (sourceName.isEmpty()) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    OBSSourceAutoRelease source = obs_get_source_by_name(sourceName.toUtf8());
    if (!source) {
        req->SendErrorResponse("specified source doesn't exist");
        return;
    }

    obs_source_set_muted(source, mute);
    req->SendOKResponse();
}

/**
 * Get the mute status of a specified source.
 *
 * @param {String} `source` The name of the source.
 *
 * @return {String} `name` The name of the source.
 * @return {boolean} `muted` Mute status of the source.
 *
 * @api requests
 * @name GetMute
 * @category sources
 * @since 4.0.0
 */
void WSRequestHandler::HandleGetMute(WSRequestHandler* req) {
    if (!req->hasField("source")) {
        req->SendErrorResponse("mssing request parameters");
        return;
    }

    QString sourceName = obs_data_get_string(req->data, "source");
    if (sourceName.isEmpty()) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    OBSSourceAutoRelease source = obs_get_source_by_name(sourceName.toUtf8());
    if (!source) {
        req->SendErrorResponse("specified source doesn't exist");
        return;
    }

    OBSDataAutoRelease response = obs_data_create();
    obs_data_set_string(response, "name", obs_source_get_name(source));
    obs_data_set_bool(response, "muted", obs_source_muted(source));

    req->SendOKResponse(response);
}

/**
 * Set the audio sync offset of a specified source.
 * 
 * @param {String} `source` The name of the source.
 * @param {int} `offset` The desired audio sync offset (in nanoseconds).
 * 
 * @api requests
 * @name SetSyncOffset
 * @category sources
 * @since 4.2.0
 */
void WSRequestHandler::HandleSetSyncOffset(WSRequestHandler* req) {
    if (!req->hasField("source") || !req->hasField("offset")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    QString sourceName = obs_data_get_string(req->data, "source");
    int64_t sourceSyncOffset = (int64_t)obs_data_get_int(req->data, "offset");

    if (sourceName.isEmpty() || sourceSyncOffset < 0) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    OBSSourceAutoRelease source = obs_get_source_by_name(sourceName.toUtf8());
    if (!source) {
        req->SendErrorResponse("specified source doesn't exist");
        return;
    }

    obs_source_set_sync_offset(source, sourceSyncOffset);
    req->SendOKResponse();
}

/**
 * Get the audio sync offset of a specified source.
 * 
 * @param {String} `source` The name of the source.
 * 
 * @return {String} `name` The name of the source.
 * @return {int} `offset` The audio sync offset (in nanoseconds).
 * 
 * @api requests
 * @name GetSyncOffset
 * @category sources
 * @since 4.2.0
 */
void WSRequestHandler::HandleGetSyncOffset(WSRequestHandler* req) {
    if (!req->hasField("source")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    QString sourceName = obs_data_get_string(req->data, "source");
    if (!sourceName.isEmpty()) {
        OBSSourceAutoRelease source = obs_get_source_by_name(sourceName.toUtf8());

        OBSDataAutoRelease response = obs_data_create();
        obs_data_set_string(response, "name", sourceName.toUtf8());
        obs_data_set_int(response, "offset", obs_source_get_sync_offset(source));

        req->SendOKResponse(response);
    } else {
        req->SendErrorResponse("invalid request parameters");
    }
}

/**
 * Sets the coordinates of a specified source item.
 *
 * @param {String (optional)} `scene-name` The name of the scene that the source item belongs to. Defaults to the current scene.
 * @param {String} `item` The name of the source item.
 * @param {double} `x` X coordinate.
 * @param {double} `y` Y coordinate.
 
 *
 * @api requests
 * @name SetSceneItemPosition
 * @category sources
 * @since 4.0.0
 * @deprecated Since unreleased. Prefer the use of SetSceneItemProperties.
 */
 void WSRequestHandler::HandleSetSceneItemPosition(WSRequestHandler* req) {
    if (!req->hasField("item") ||
        !req->hasField("x") || !req->hasField("y")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    QString itemName = obs_data_get_string(req->data, "item");
    if (itemName.isEmpty()) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    QString sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene could not be found");
        return;
    }

    OBSSceneItem sceneItem = Utils::GetSceneItemFromName(scene, itemName);
    if (sceneItem) {
        vec2 item_position = { 0 };
        item_position.x = obs_data_get_double(req->data, "x");
        item_position.y = obs_data_get_double(req->data, "y");
        obs_sceneitem_set_pos(sceneItem, &item_position);

        req->SendOKResponse();
    } else {
        req->SendErrorResponse("specified scene item doesn't exist");
    }
}

/**
 * Set the transform of the specified source item.
 *
 * @param {String (optional)} `scene-name` The name of the scene that the source item belongs to. Defaults to the current scene.
 * @param {String} `item` The name of the source item.
 * @param {double} `x-scale` Width scale factor.
 * @param {double} `y-scale` Height scale factor.
 * @param {double} `rotation` Source item rotation (in degrees). 
 *
 * @api requests
 * @name SetSceneItemTransform
 * @category sources
 * @since 4.0.0
 * @deprecated Since unreleased. Prefer the use of SetSceneItemProperties.
 */
void WSRequestHandler::HandleSetSceneItemTransform(WSRequestHandler* req) {
    if (!req->hasField("item") ||
        !req->hasField("x-scale") ||
        !req->hasField("y-scale") ||
        !req->hasField("rotation"))
    {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    QString itemName = obs_data_get_string(req->data, "item");
    if (itemName.isEmpty()) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    QString sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene doesn't exist");
        return;
    }

    vec2 scale;
    scale.x = obs_data_get_double(req->data, "x-scale");
    scale.y = obs_data_get_double(req->data, "y-scale");
    float rotation = obs_data_get_double(req->data, "rotation");

    OBSSceneItemAutoRelease sceneItem = Utils::GetSceneItemFromName(scene, itemName);
    if (sceneItem) {
        obs_sceneitem_set_scale(sceneItem, &scale);
        obs_sceneitem_set_rot(sceneItem, rotation);
        req->SendOKResponse();
    } else {
        req->SendErrorResponse("specified scene item doesn't exist");
    }
}

/**
 * Sets the crop coordinates of the specified source item.
 *
 * @param {String (optional)} `scene-name` the name of the scene that the source item belongs to. Defaults to the current scene.
 * @param {String} `item` The name of the source.
 * @param {int} `top` Pixel position of the top of the source item.
 * @param {int} `bottom` Pixel position of the bottom of the source item.
 * @param {int} `left` Pixel position of the left of the source item.
 * @param {int} `right` Pixel position of the right of the source item.
 *
 * @api requests
 * @name SetSceneItemCrop
 * @category sources
 * @since 4.1.0
 * @deprecated Since unreleased. Prefer the use of SetSceneItemProperties.
 */
void WSRequestHandler::HandleSetSceneItemCrop(WSRequestHandler* req) {
    if (!req->hasField("item")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    QString itemName = obs_data_get_string(req->data, "item");
    if (itemName.isEmpty()) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    QString sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene doesn't exist");
        return;
    }

    OBSSceneItemAutoRelease sceneItem = Utils::GetSceneItemFromName(scene, itemName);
    if (sceneItem) {
        struct obs_sceneitem_crop crop = { 0 };
        crop.top = obs_data_get_int(req->data, "top");
        crop.bottom = obs_data_get_int(req->data, "bottom");
        crop.left = obs_data_get_int(req->data, "left");
        crop.right = obs_data_get_int(req->data, "right");

        obs_sceneitem_set_crop(sceneItem, &crop);

        req->SendOKResponse();
    } else {
        req->SendErrorResponse("specified scene item doesn't exist");
    }
}

/**
 * Gets the scene specific properties of the specified source item.
 *
 * @param {String (optional)} `scene-name` the name of the scene that the source item belongs to. Defaults to the current scene.
 * @param {String} `item` The name of the source.
 *
 * @return {String} `name` The name of the source.
 * @return {int} `position.x` The x position of the source from the left.
 * @return {int} `position.y` The y position of the source from the top.
 * @return {int} `position.alignment` The point on the source that the item is manipulated from.
 * @return {double} `rotation` The clockwise rotation of the item in degrees around the point of alignment.
 * @return {double} `scale.x` The x-scale factor of the source.
 * @return {double} `scale.y` The y-scale factor of the source.
 * @return {int} `crop.top` The number of pixels cropped off the top of the source before scaling.
 * @return {int} `crop.right` The number of pixels cropped off the right of the source before scaling.
 * @return {int} `crop.bottom` The number of pixels cropped off the bottom of the source before scaling.
 * @return {int} `crop.left` The number of pixels cropped off the left of the source before scaling.
 * @return {bool} `visible` If the source is visible.
 * @return {String} `bounds.type` Type of bounding box.
 * @return {int} `bounds.alignment` Alignment of the bounding box.
 * @return {double} `bounds.x` Width of the bounding box.
 * @return {double} `bounds.y` Height of the bounding box.
 *
 * @api requests
 * @name GetSceneItemProperties
 * @category sources
 * @since unreleased
 */
void WSRequestHandler::HandleGetSceneItemProperties(WSRequestHandler* req) {
    if (!req->hasField("item")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    QString itemName = obs_data_get_string(req->data, "item");
    if (itemName.isEmpty()) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    QString sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene doesn't exist");
        return;
    }

    OBSSceneItemAutoRelease sceneItem =
        Utils::GetSceneItemFromName(scene, itemName);
    if (!sceneItem) {
        req->SendErrorResponse("specified scene item doesn't exist");
        return;
    }

    OBSDataAutoRelease data = obs_data_create();
    obs_data_set_string(data, "name", itemName.toUtf8());

    OBSDataAutoRelease posData = obs_data_create();
    vec2 pos;
    obs_sceneitem_get_pos(sceneItem, &pos);
    obs_data_set_double(posData, "x", pos.x);
    obs_data_set_double(posData, "y", pos.y);
    obs_data_set_int(posData, "alignment", obs_sceneitem_get_alignment(sceneItem));
    obs_data_set_obj(data, "position", posData);

    obs_data_set_double(data, "rotation", obs_sceneitem_get_rot(sceneItem));

    OBSDataAutoRelease scaleData = obs_data_create();
    vec2 scale;
    obs_sceneitem_get_scale(sceneItem, &scale);
    obs_data_set_double(scaleData, "x", scale.x);
    obs_data_set_double(scaleData, "y", scale.y);
    obs_data_set_obj(data, "scale", scaleData);

    OBSDataAutoRelease cropData = obs_data_create();
    obs_sceneitem_crop crop;
    obs_sceneitem_get_crop(sceneItem, &crop);
    obs_data_set_int(cropData, "left", crop.left);
    obs_data_set_int(cropData, "top", crop.top);
    obs_data_set_int(cropData, "right", crop.right);
    obs_data_set_int(cropData, "bottom", crop.bottom);
    obs_data_set_obj(data, "crop", cropData);

    obs_data_set_bool(data, "visible", obs_sceneitem_visible(sceneItem));

    OBSDataAutoRelease boundsData = obs_data_create();
    obs_bounds_type boundsType = obs_sceneitem_get_bounds_type(sceneItem);
    if (boundsType == OBS_BOUNDS_NONE) {
        obs_data_set_string(boundsData, "type", "OBS_BOUNDS_NONE");
    }
    else {
        switch(boundsType) {
            case OBS_BOUNDS_STRETCH: {
                obs_data_set_string(boundsData, "type", "OBS_BOUNDS_STRETCH");
                break;
            }
            case OBS_BOUNDS_SCALE_INNER: {
                obs_data_set_string(boundsData, "type", "OBS_BOUNDS_SCALE_INNER");
                break;
            }
            case OBS_BOUNDS_SCALE_OUTER: {
                obs_data_set_string(boundsData, "type", "OBS_BOUNDS_SCALE_OUTER");
                break;
            }
            case OBS_BOUNDS_SCALE_TO_WIDTH: {
                obs_data_set_string(boundsData, "type", "OBS_BOUNDS_SCALE_TO_WIDTH");
                break;
            }
            case OBS_BOUNDS_SCALE_TO_HEIGHT: {
                obs_data_set_string(boundsData, "type", "OBS_BOUNDS_SCALE_TO_HEIGHT");
                break;
            }
            case OBS_BOUNDS_MAX_ONLY: {
                obs_data_set_string(boundsData, "type", "OBS_BOUNDS_MAX_ONLY");
                break;
            }
        }
        obs_data_set_int(boundsData, "alignment", obs_sceneitem_get_bounds_alignment(sceneItem));
        vec2 bounds;
        obs_sceneitem_get_bounds(sceneItem, &bounds);
        obs_data_set_double(boundsData, "x", bounds.x);
        obs_data_set_double(boundsData, "y", bounds.y);
    }
    obs_data_set_obj(data, "bounds", boundsData);

    req->SendOKResponse(data);
}

/**
 * Sets the scene specific properties of a source. Unspecified properties will remain unchanged.
 *
 * @param {String (optional)} `scene-name` the name of the scene that the source item belongs to. Defaults to the current scene.
 * @param {String} `item` The name of the source.
 * @param {int} `position.x` The new x position of the source.
 * @param {int} `position.y` The new y position of the source.
 * @param {int} `position.alignment` The new alignment of the source.
 * @param {double} `rotation` The new clockwise rotation of the item in degrees.
 * @param {double} `scale.x` The new x scale of the item.
 * @param {double} `scale.y` The new y scale of the item.
 * @param {int} `crop.top` The new amount of pixels cropped off the top of the source before scaling.
 * @param {int} `crop.bottom` The new amount of pixels cropped off the bottom of the source before scaling.
 * @param {int} `crop.left` The new amount of pixels cropped off the left of the source before scaling.
 * @param {int} `crop.right` The new amount of pixels cropped off the right of the source before scaling.
 * @param {bool} `visible` The new visibility of the source. 'true' shows source, 'false' hides source.
 * @param {String} `bounds.type` The new bounds type of the source.
 * @param {int} `bounds.alignment` The new alignment of the bounding box. (0-2, 4-6, 8-10)
 * @param {double} `bounds.x` The new width of the bounding box.
 * @param {double} `bounds.y' The new height of the bounding box.
 *
 * @api requests
 * @name SetSceneItemProperties
 * @category sources
 * @since unreleased
 */
void WSRequestHandler::HandleSetSceneItemProperties(WSRequestHandler* req) {
    if (!req->hasField("item")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    QString itemName = obs_data_get_string(req->data, "item");
    if (itemName.isEmpty()) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    QString sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene doesn't exist");
        return;
    }

    OBSSceneItemAutoRelease sceneItem =
        Utils::GetSceneItemFromName(scene, itemName);
    if (!sceneItem) {
        req->SendErrorResponse("specified scene item doesn't exist");
        return;
    }

    bool badRequest = false;
    OBSDataAutoRelease errorMessage = obs_data_create();

    if (req->hasField("position")) {
        vec2 oldPosition;
        OBSDataAutoRelease positionError = obs_data_create();
        obs_sceneitem_get_pos(sceneItem, &oldPosition);
        OBSDataAutoRelease reqPosition = obs_data_get_obj(req->data, "position");
        vec2 newPosition = oldPosition;
        if (obs_data_has_user_value(reqPosition, "x")) {
            newPosition.x = obs_data_get_int(reqPosition, "x");
        }
        if (obs_data_has_user_value(reqPosition, "y")) {
            newPosition.y = obs_data_get_int(reqPosition, "y");
        }
        if (obs_data_has_user_value(reqPosition, "alignment")) {
            const uint32_t alignment = obs_data_get_int(reqPosition, "alignment");
            if (Utils::IsValidAlignment(alignment)) {
                obs_sceneitem_set_alignment(sceneItem, alignment);
            } else {
                badRequest = true;
                obs_data_set_string(positionError, "alignment", "invalid");
                obs_data_set_obj(errorMessage, "position", positionError);
            }
        }
        obs_sceneitem_set_pos(sceneItem, &newPosition);
    }

    if (req->hasField("rotation")) {
        obs_sceneitem_set_rot(sceneItem, (float)obs_data_get_double(req->data, "rotation"));
    }

    if (req->hasField("scale")) {
        vec2 oldScale;
        obs_sceneitem_get_scale(sceneItem, &oldScale);
        OBSDataAutoRelease reqScale = obs_data_get_obj(req->data, "scale");
        vec2 newScale = oldScale;
        if (obs_data_has_user_value(reqScale, "x")) {
            newScale.x = obs_data_get_double(reqScale, "x");
        }
        if (obs_data_has_user_value(reqScale, "y")) {
            newScale.y = obs_data_get_double(reqScale, "y");
        }
        obs_sceneitem_set_scale(sceneItem, &newScale);
    }

    if (req->hasField("crop")) {
        obs_sceneitem_crop oldCrop;
        obs_sceneitem_get_crop(sceneItem, &oldCrop);
        OBSDataAutoRelease reqCrop = obs_data_get_obj(req->data, "crop");
        obs_sceneitem_crop newCrop = oldCrop;
        if (obs_data_has_user_value(reqCrop, "top")) {
            newCrop.top = obs_data_get_int(reqCrop, "top");
        }
        if (obs_data_has_user_value(reqCrop, "right")) {
            newCrop.right = obs_data_get_int(reqCrop, "right");
        }
        if (obs_data_has_user_value(reqCrop, "bottom")) {
            newCrop.bottom = obs_data_get_int(reqCrop, "bottom");
        }
        if (obs_data_has_user_value(reqCrop, "left")) {
            newCrop.left = obs_data_get_int(reqCrop, "left");
        }
        obs_sceneitem_set_crop(sceneItem, &newCrop);
    }

    if (req->hasField("visible")) {
        obs_sceneitem_set_visible(sceneItem, obs_data_get_bool(req->data, "visible"));
    }

    if (req->hasField("bounds")) {
        bool badBounds = false;
        OBSDataAutoRelease boundsError = obs_data_create();
        OBSDataAutoRelease reqBounds = obs_data_get_obj(req->data, "bounds");
        if (obs_data_has_user_value(reqBounds, "type")) {
            const char* newBoundsType = obs_data_get_string(reqBounds, "type");
            if (newBoundsType == "OBS_BOUNDS_NONE") {
                obs_sceneitem_set_bounds_type(sceneItem, OBS_BOUNDS_NONE);
            }
            else if (newBoundsType == "OBS_BOUNDS_STRETCH") {
                obs_sceneitem_set_bounds_type(sceneItem, OBS_BOUNDS_STRETCH);
            }
            else if (newBoundsType == "OBS_BOUNDS_SCALE_INNER") {
                obs_sceneitem_set_bounds_type(sceneItem, OBS_BOUNDS_SCALE_INNER);
            }
            else if (newBoundsType == "OBS_BOUNDS_SCALE_OUTER") {
                obs_sceneitem_set_bounds_type(sceneItem, OBS_BOUNDS_SCALE_OUTER);
            }
            else if (newBoundsType == "OBS_BOUNDS_SCALE_TO_WIDTH") {
                obs_sceneitem_set_bounds_type(sceneItem, OBS_BOUNDS_SCALE_TO_WIDTH);
            }
            else if (newBoundsType == "OBS_BOUNDS_SCALE_TO_HEIGHT") {
                obs_sceneitem_set_bounds_type(sceneItem, OBS_BOUNDS_SCALE_TO_HEIGHT);
            }
            else if (newBoundsType == "OBS_BOUNDS_MAX_ONLY") {
                obs_sceneitem_set_bounds_type(sceneItem, OBS_BOUNDS_MAX_ONLY);
            }
            else {
                badRequest = badBounds = true;
                obs_data_set_string(boundsError, "type", "invalid");
            }
        }
        vec2 oldBounds;
        obs_sceneitem_get_bounds(sceneItem, &oldBounds);
        vec2 newBounds = oldBounds;
        if (obs_data_has_user_value(reqBounds, "x")) {
            newBounds.x = obs_data_get_double(reqBounds, "x");
        }
        if (obs_data_has_user_value(reqBounds, "y")) {
            newBounds.y = obs_data_get_double(reqBounds, "y");
        }
        obs_sceneitem_set_bounds(sceneItem, &newBounds);
        if (obs_data_has_user_value(reqBounds, "alignment")) {
            const uint32_t bounds_alignment = obs_data_get_int(reqBounds, "alignment");
            if (Utils::IsValidAlignment(bounds_alignment)) {
                obs_sceneitem_set_bounds_alignment(sceneItem, bounds_alignment);
            } else {
                badRequest = badBounds = true;
                obs_data_set_string(boundsError, "alignment", "invalid");
            }
        }
        if (badBounds) {
            obs_data_set_obj(errorMessage, "bounds", boundsError);
        }
    }

    if (badRequest) {
        req->SendErrorResponse(errorMessage);
    } else {
        req->SendOKResponse();
    }
}

/**
 * Get the current properties of a Text GDI Plus source.
 *
 * @param {String (optional)} `scene-name` Name of the scene to retrieve. Defaults to the current scene.
 * @param {String} `source` Name of the source.
 *
 * @return {String} `align` Text Alignment ("left", "center", "right").
 * @return {int} `bk-color` Background color.
 * @return {int} `bk-opacity` Background opacity (0-100).
 * @return {boolean} `chatlog` Chat log.
 * @return {int} `chatlog_lines` Chat log lines.
 * @return {int} `color` Text color.
 * @return {boolean} `extents` Extents wrap.
 * @return {int} `extents_cx` Extents cx.
 * @return {int} `extents_cy` Extents cy.
 * @return {String} `file` File path name.
 * @return {boolean} `read_from_file` Read text from the specified file.
 * @return {Object} `font` Holds data for the font. Ex: `"font": { "face": "Arial", "flags": 0, "size": 150, "style": "" }`
 * @return {String} `font.face` Font face.
 * @return {int} `font.flags` Font text styling flag. `Bold=1, Italic=2, Bold Italic=3, Underline=5, Strikeout=8`
 * @return {int} `font.size` Font text size.
 * @return {String} `font.style` Font Style (unknown function).
 * @return {boolean} `gradient` Gradient enabled.
 * @return {int} `gradient_color` Gradient color.
 * @return {float} `gradient_dir` Gradient direction.
 * @return {int} `gradient_opacity` Gradient opacity (0-100).
 * @return {boolean} `outline` Outline.
 * @return {int} `outline_color` Outline color.
 * @return {int} `outline_size` Outline size.
 * @return {int} `outline_opacity` Outline opacity (0-100).
 * @return {String} `text` Text content to be displayed.
 * @return {String} `valign` Text vertical alignment ("top", "center", "bottom").
 * @return {boolean} `vertical` Vertical text enabled.
 * @return {boolean} `render` Visibility of the scene item.
 *
 * @api requests
 * @name GetTextGDIPlusProperties
 * @category sources
 * @since 4.1.0
 */
 void WSRequestHandler::HandleGetTextGDIPlusProperties(WSRequestHandler* req) {
    const char* itemName = obs_data_get_string(req->data, "source");
    if (!itemName) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    const char* sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene doesn't exist");
        return;
    }

    OBSSceneItemAutoRelease sceneItem = Utils::GetSceneItemFromName(scene, itemName);
    if (sceneItem) {
        OBSSource sceneItemSource = obs_sceneitem_get_source(sceneItem);
        const char* sceneItemSourceId = obs_source_get_id(sceneItemSource);

        if (strcmp(sceneItemSourceId, "text_gdiplus") == 0) {
            OBSDataAutoRelease response = obs_source_get_settings(sceneItemSource);
            obs_data_set_string(response, "source", itemName);
            obs_data_set_string(response, "scene-name", sceneName);
            obs_data_set_bool(response, "render",
                obs_sceneitem_visible(sceneItem));

            req->SendOKResponse(response);
        } else {
            req->SendErrorResponse("not text gdi plus source");
        }
    } else {
        req->SendErrorResponse("specified scene item doesn't exist");
    }
}

/**
 * Get the current properties of a Text GDI Plus source.
 *
 * @param {String (optional)} `scene-name` Name of the scene to retrieve. Defaults to the current scene.
 * @param {String} `source` Name of the source.
 * @param {String (optional)} `align` Text Alignment ("left", "center", "right").
 * @param {int (optional)} `bk-color` Background color.
 * @param {int (optional)} `bk-opacity` Background opacity (0-100).
 * @param {boolean (optional)} `chatlog` Chat log.
 * @param {int (optional)} `chatlog_lines` Chat log lines.
 * @param {int (optional)} `color` Text color.
 * @param {boolean (optional)} `extents` Extents wrap.
 * @param {int (optional)} `extents_cx` Extents cx.
 * @param {int (optional)} `extents_cy` Extents cy.
 * @param {String (optional)} `file` File path name.
 * @param {boolean (optional)} `read_from_file` Read text from the specified file.
 * @param {Object (optional)} `font` Holds data for the font. Ex: `"font": { "face": "Arial", "flags": 0, "size": 150, "style": "" }`
 * @param {String (optional)} `font.face` Font face.
 * @param {int (optional)} `font.flags` Font text styling flag. `Bold=1, Italic=2, Bold Italic=3, Underline=5, Strikeout=8`
 * @param {int (optional)} `font.size` Font text size.
 * @param {String (optional)} `font.style` Font Style (unknown function).
 * @param {boolean (optional)} `gradient` Gradient enabled.
 * @param {int (optional)} `gradient_color` Gradient color.
 * @param {float (optional)} `gradient_dir` Gradient direction.
 * @param {int (optional)} `gradient_opacity` Gradient opacity (0-100).
 * @param {boolean (optional)} `outline` Outline.
 * @param {int (optional)} `outline_color` Outline color.
 * @param {int (optional)} `outline_size` Outline size.
 * @param {int (optional)} `outline_opacity` Outline opacity (0-100).
 * @param {String (optional)} `text` Text content to be displayed.
 * @param {String (optional)} `valign` Text vertical alignment ("top", "center", "bottom").
 * @param {boolean (optional)} `vertical` Vertical text enabled.
 * @param {boolean (optional)} `render` Visibility of the scene item.
 *
 * @api requests
 * @name SetTextGDIPlusProperties
 * @category sources
 * @since 4.1.0
 */
void WSRequestHandler::HandleSetTextGDIPlusProperties(WSRequestHandler* req) {
    if (!req->hasField("source")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    const char* itemName = obs_data_get_string(req->data, "source");
    if (!itemName) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    const char* sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene doesn't exist");
        return;
    }

    OBSSceneItemAutoRelease sceneItem = Utils::GetSceneItemFromName(scene, itemName);
    if (sceneItem) {
        OBSSource sceneItemSource = obs_sceneitem_get_source(sceneItem);
        const char* sceneItemSourceId = obs_source_get_id(sceneItemSource);

        if (strcmp(sceneItemSourceId, "text_gdiplus") == 0) {
            OBSDataAutoRelease settings = obs_source_get_settings(sceneItemSource);

            if (req->hasField("align")) {
                obs_data_set_string(settings, "align",
                    obs_data_get_string(req->data, "align"));
            }

            if (req->hasField("bk_color")) {
                obs_data_set_int(settings, "bk_color",
                    obs_data_get_int(req->data, "bk_color"));
            }

            if (req->hasField("bk-opacity")) {
                obs_data_set_int(settings, "bk_opacity",
                    obs_data_get_int(req->data, "bk_opacity"));
            }

            if (req->hasField("chatlog")) {
                obs_data_set_bool(settings, "chatlog",
                    obs_data_get_bool(req->data, "chatlog"));
            }

            if (req->hasField("chatlog_lines")) {
                obs_data_set_int(settings, "chatlog_lines",
                    obs_data_get_int(req->data, "chatlog_lines"));
            }

            if (req->hasField("color")) {
                obs_data_set_int(settings, "color",
                    obs_data_get_int(req->data, "color"));
            }

            if (req->hasField("extents")) {
                obs_data_set_bool(settings, "extents",
                    obs_data_get_bool(req->data, "extents"));
            }

            if (req->hasField("extents_wrap")) {
                obs_data_set_bool(settings, "extents_wrap",
                    obs_data_get_bool(req->data, "extents_wrap"));
            }

            if (req->hasField("extents_cx")) {
                obs_data_set_int(settings, "extents_cx",
                    obs_data_get_int(req->data, "extents_cx"));
            }

            if (req->hasField("extents_cy")) {
                obs_data_set_int(settings, "extents_cy",
                    obs_data_get_int(req->data, "extents_cy"));
            }

            if (req->hasField("file")) {
                obs_data_set_string(settings, "file",
                    obs_data_get_string(req->data, "file"));
            }

            if (req->hasField("font")) {
                OBSDataAutoRelease font_obj = obs_data_get_obj(settings, "font");
                if (font_obj) {
                    OBSDataAutoRelease req_font_obj = obs_data_get_obj(req->data, "font");

                    if (obs_data_has_user_value(req_font_obj, "face")) {
                        obs_data_set_string(font_obj, "face",
                            obs_data_get_string(req_font_obj, "face"));
                    }

                    if (obs_data_has_user_value(req_font_obj, "flags")) {
                        obs_data_set_int(font_obj, "flags",
                            obs_data_get_int(req_font_obj, "flags"));
                    }

                    if (obs_data_has_user_value(req_font_obj, "size")) {
                        obs_data_set_int(font_obj, "size",
                            obs_data_get_int(req_font_obj, "size"));
                    }

                    if (obs_data_has_user_value(req_font_obj, "style")) {
                        obs_data_set_string(font_obj, "style",
                            obs_data_get_string(req_font_obj, "style"));
                    }
                }
            }

            if (req->hasField("gradient")) {
                obs_data_set_bool(settings, "gradient",
                    obs_data_get_bool(req->data, "gradient"));
            }

            if (req->hasField("gradient_color")) {
                obs_data_set_int(settings, "gradient_color",
                    obs_data_get_int(req->data, "gradient_color"));
            }

            if (req->hasField("gradient_dir")) {
                obs_data_set_double(settings, "gradient_dir",
                    obs_data_get_double(req->data, "gradient_dir"));
            }

            if (req->hasField("gradient_opacity")) {
                obs_data_set_int(settings, "gradient_opacity",
                    obs_data_get_int(req->data, "gradient_opacity"));
            }

            if (req->hasField("outline")) {
                obs_data_set_bool(settings, "outline",
                    obs_data_get_bool(req->data, "outline"));
            }

            if (req->hasField("outline_size")) {
                obs_data_set_int(settings, "outline_size",
                    obs_data_get_int(req->data, "outline_size"));
            }

            if (req->hasField("outline_color")) {
                obs_data_set_int(settings, "outline_color",
                    obs_data_get_int(req->data, "outline_color"));
            }

            if (req->hasField("outline_opacity")) {
                obs_data_set_int(settings, "outline_opacity",
                    obs_data_get_int(req->data, "outline_opacity"));
            }

            if (req->hasField("read_from_file")) {
                obs_data_set_bool(settings, "read_from_file",
                    obs_data_get_bool(req->data, "read_from_file"));
            }

            if (req->hasField("text")) {
                obs_data_set_string(settings, "text",
                    obs_data_get_string(req->data, "text"));
            }

            if (req->hasField("valign")) {
                obs_data_set_string(settings, "valign",
                    obs_data_get_string(req->data, "valign"));
            }

            if (req->hasField("vertical")) {
                obs_data_set_bool(settings, "vertical",
                    obs_data_get_bool(req->data, "vertical"));
            }

            obs_source_update(sceneItemSource, settings);

            if (req->hasField("render")) {
                obs_sceneitem_set_visible(sceneItem,
                    obs_data_get_bool(req->data, "render"));
            }

            req->SendOKResponse();
        } else {
            req->SendErrorResponse("not text gdi plus source");
        }
    } else {
        req->SendErrorResponse("specified scene item doesn't exist");
    }
}

/**
 * Get current properties for a Browser Source.
 *
 * @param {String (optional)} `scene-name` Name of the scene that the source belongs to. Defaults to the current scene.
 * @param {String} `source` Name of the source.
 *
 * @return {boolean} `is_local_file` Indicates that a local file is in use.
 * @return {String} `url` Url or file path.
 * @return {String} `css` CSS to inject.
 * @return {int} `width` Width.
 * @return {int} `height` Height.
 * @return {int} `fps` Framerate.
 * @return {boolean} `shutdown` Indicates whether the source should be shutdown when not visible.
 * @return {boolean (optional)} `render` Visibility of the scene item.
 *
 * @api requests
 * @name GetBrowserSourceProperties
 * @category sources
 * @since 4.1.0
 */
void WSRequestHandler::HandleGetBrowserSourceProperties(WSRequestHandler* req) {
    const char* itemName = obs_data_get_string(req->data, "source");
    if (!itemName) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    const char* sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene doesn't exist");
        return;
    }

    OBSSceneItemAutoRelease sceneItem = Utils::GetSceneItemFromName(scene, itemName);
    if (sceneItem) {
        OBSSource sceneItemSource = obs_sceneitem_get_source(sceneItem);
        const char* sceneItemSourceId = obs_source_get_id(sceneItemSource);

        if (strcmp(sceneItemSourceId, "browser_source") == 0) {
            OBSDataAutoRelease response = obs_source_get_settings(sceneItemSource);
            obs_data_set_string(response, "source", itemName);
            obs_data_set_string(response, "scene-name", sceneName);
            obs_data_set_bool(response, "render",
                obs_sceneitem_visible(sceneItem));

            req->SendOKResponse(response);
        } else {
            req->SendErrorResponse("not browser source");
        }
    } else {
        req->SendErrorResponse("specified scene item doesn't exist");
    }
}

/**
 * Set current properties for a Browser Source.
 *
 * @param {String (optional)} `scene-name` Name of the scene that the source belongs to. Defaults to the current scene.
 * @param {String} `source` Name of the source.
 * @param {boolean (optional)} `is_local_file` Indicates that a local file is in use.
 * @param {String (optional)} `url` Url or file path.
 * @param {String (optional)} `css` CSS to inject.
 * @param {int (optional)} `width` Width.
 * @param {int (optional)} `height` Height.
 * @param {int (optional)} `fps` Framerate.
 * @param {boolean (optional)} `shutdown` Indicates whether the source should be shutdown when not visible.
 * @param {boolean (optional)} `render` Visibility of the scene item.
 *
 * @api requests
 * @name SetBrowserSourceProperties
 * @category sources
 * @since 4.1.0
 */
void WSRequestHandler::HandleSetBrowserSourceProperties(WSRequestHandler* req) {
    if (!req->hasField("source")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    const char* itemName = obs_data_get_string(req->data, "source");
    if (!itemName) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    const char* sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene doesn't exist");
        return;
    }

    OBSSceneItemAutoRelease sceneItem = Utils::GetSceneItemFromName(scene, itemName);
    if (sceneItem) {
        OBSSource sceneItemSource = obs_sceneitem_get_source(sceneItem);
        const char* sceneItemSourceId = obs_source_get_id(sceneItemSource);

        if (strcmp(sceneItemSourceId, "browser_source") == 0) {
            OBSDataAutoRelease settings = obs_source_get_settings(sceneItemSource);

            if (req->hasField("restart_when_active")) {
                obs_data_set_bool(settings, "restart_when_active",
                    obs_data_get_bool(req->data, "restart_when_active"));
            }

            if (req->hasField("shutdown")) {
                obs_data_set_bool(settings, "shutdown",
                    obs_data_get_bool(req->data, "shutdown"));
            }

            if (req->hasField("is_local_file")) {
                obs_data_set_bool(settings, "is_local_file",
                    obs_data_get_bool(req->data, "is_local_file"));
            }

            if (req->hasField("url")) {
                obs_data_set_string(settings, "url",
                    obs_data_get_string(req->data, "url"));
            }

            if (req->hasField("css")) {
                obs_data_set_string(settings, "css",
                    obs_data_get_string(req->data, "css"));
            }

            if (req->hasField("width")) {
                obs_data_set_int(settings, "width",
                    obs_data_get_int(req->data, "width"));
            }

            if (req->hasField("height")) {
                obs_data_set_int(settings, "height",
                    obs_data_get_int(req->data, "height"));
            }

            if (req->hasField("fps")) {
                obs_data_set_int(settings, "fps",
                    obs_data_get_int(req->data, "fps"));
            }

            obs_source_update(sceneItemSource, settings);

            if (req->hasField("render")) {
                obs_sceneitem_set_visible(sceneItem,
                    obs_data_get_bool(req->data, "render"));
            }

            req->SendOKResponse();
        } else {
            req->SendErrorResponse("not browser source");
        }
    } else {
        req->SendErrorResponse("specified scene item doesn't exist");
    }
}

/**
 * Reset a source item.
 *
 * @param {String (optional)} `scene-name` Name of the scene the source belogns to. Defaults to the current scene.
 * @param {String} `item` Name of the source item.
 *
 * @api requests
 * @name ResetSceneItem
 * @category sources
 * @since 4.2.0
 */
void WSRequestHandler::HandleResetSceneItem(WSRequestHandler* req) {
    if (!req->hasField("item")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    const char* itemName = obs_data_get_string(req->data, "item");
    if (!itemName) {
        req->SendErrorResponse("invalid request parameters");
        return;
    }

    const char* sceneName = obs_data_get_string(req->data, "scene-name");
    OBSSourceAutoRelease scene = Utils::GetSceneFromNameOrCurrent(sceneName);
    if (!scene) {
        req->SendErrorResponse("requested scene doesn't exist");
        return;
    }

    OBSSceneItemAutoRelease sceneItem = Utils::GetSceneItemFromName(scene, itemName);
    if (sceneItem) {
        OBSSource sceneItemSource = obs_sceneitem_get_source(sceneItem);

        OBSDataAutoRelease settings = obs_source_get_settings(sceneItemSource);
        obs_source_update(sceneItemSource, settings);

        req->SendOKResponse();
    } else {
        req->SendErrorResponse("specified scene item doesn't exist");
    }
}

/**
 * List all sources available in the running OBS instance
 *
 * @return {Array of Objects} `sources` Array of sources as objects
 * @return {String} `sources.*.name` Unique source name
 * @return {String} `sources.*.typeId` Non-unique source internal type (a.k.a type id)
 * @return {String} `sources.*.type` Source type. Value is one of the following: "input", "filter", "transition", "scene" or "unknown"
 *
 * @api requests
 * @name GetSourcesList
 * @category sources
 * @since unreleased
 */
 void WSRequestHandler::HandleGetSourcesList(WSRequestHandler* req) {
    OBSDataArrayAutoRelease sourcesArray = obs_data_array_create();

    auto sourceEnumProc = [](void* privateData, obs_source_t* source) -> bool {
        obs_data_array_t* sourcesArray = (obs_data_array_t*)privateData;

        OBSDataAutoRelease sourceData = obs_data_create();
        obs_data_set_string(sourceData, "name", obs_source_get_name(source));
        obs_data_set_string(sourceData, "typeId", obs_source_get_id(source));

        QString typeString = "";
        enum obs_source_type sourceType = obs_source_get_type(source);
        switch (sourceType) {
            case OBS_SOURCE_TYPE_INPUT:
                typeString = "input";
                break;

            case OBS_SOURCE_TYPE_FILTER:
                typeString = "filter";
                break;

            case OBS_SOURCE_TYPE_TRANSITION:
                typeString = "transition";
                break;

            case OBS_SOURCE_TYPE_SCENE:
                typeString = "scene";
                break;

            default:
                typeString = "unknown";
                break;
        }
        obs_data_set_string(sourceData, "type", typeString.toUtf8());

        obs_data_array_push_back(sourcesArray, sourceData);
        return true;
    };
    obs_enum_sources(sourceEnumProc, sourcesArray);

    OBSDataAutoRelease response = obs_data_create();
    obs_data_set_array(response, "sources", sourcesArray);
    req->SendOKResponse(response);
}

/**
* Get a list of all available sources types
*
* @return {Array of Objects} `ids` Array of sources as objects
* @return {String} `ids.*.typeId` Non-unique internal source type ID
* @return {String} `ids.*.displayName` Display name of the source type
* @return {String} `ids.*.type` Type. Value is one of the following: "input", "filter", "transition" or "other"
* @return {Object} `ids.*.defaultSettings` Default settings of this source type
* @return {Object} `ids.*.caps` Source type capabilities
* @return {Boolean} `ids.*.caps.isAsync` True if source of this type provide frames asynchronously
* @return {Boolean} `ids.*.caps.hasVideo` True if sources of this type provide video
* @return {Boolean} `ids.*.caps.hasAudio` True if sources of this type provide audio
* @return {Boolean} `ids.*.caps.canInteract` True if interaction with this sources of this type is possible
* @return {Boolean} `ids.*.caps.isComposite` True if sources of this type composite one or more sub-sources
* @return {Boolean} `ids.*.caps.doNotDuplicate` True if sources of this type should not be fully duplicated
* @return {Boolean} `ids.*.caps.doNotSelfMonitor` True if sources of this type may cause a feedback loop if it's audio is monitored and shouldn't be
*
* @api requests
* @name GetSourcesTypesList
* @category sources
* @since unreleased
*/
void WSRequestHandler::HandleGetSourceTypesList(WSRequestHandler* req) {
    OBSDataArrayAutoRelease idsArray = obs_data_array_create();

    const char* id;
    size_t idx = 0;

    QHash<QString, QString> idTypes;

    idx = 0;
    while (obs_enum_input_types(idx++, &id)) {
        idTypes.insert(id, "input");
    }

    idx = 0;
    while (obs_enum_filter_types(idx++, &id)) {
        idTypes.insert(id, "filter");
    }

    idx = 0;
    while (obs_enum_transition_types(idx++, &id)) {
        idTypes.insert(id, "transition");
    }

    idx = 0;
    while (obs_enum_source_types(idx++, &id)) {
        OBSDataAutoRelease item = obs_data_create();

        obs_data_set_string(item, "typeId", id);
        obs_data_set_string(item, "displayName", obs_source_get_display_name(id));
        obs_data_set_string(item, "type", idTypes.value(id, "other").toUtf8());

        uint32_t caps = obs_get_source_output_flags(id);
        OBSDataAutoRelease capsData = obs_data_create();
        obs_data_set_bool(capsData, "isAsync", caps & OBS_SOURCE_ASYNC);
        obs_data_set_bool(capsData, "hasVideo", caps & OBS_SOURCE_VIDEO);
        obs_data_set_bool(capsData, "hasAudio", caps & OBS_SOURCE_AUDIO);
        obs_data_set_bool(capsData, "canInteract", caps & OBS_SOURCE_INTERACTION);
        obs_data_set_bool(capsData, "isComposite", caps & OBS_SOURCE_COMPOSITE);
        obs_data_set_bool(capsData, "doNotDuplicate", caps & OBS_SOURCE_DO_NOT_DUPLICATE);
        obs_data_set_bool(capsData, "doNotSelfMonitor", caps & OBS_SOURCE_DO_NOT_SELF_MONITOR);
        obs_data_set_bool(capsData, "isDeprecated", caps & OBS_SOURCE_DEPRECATED);

        obs_data_set_obj(item, "caps", capsData);

        OBSDataAutoRelease defaultSettings = obs_get_source_defaults(id);
        obs_data_set_obj(item, "defaultSettings", defaultSettings);

        obs_data_array_push_back(idsArray, item);
    }

    OBSDataAutoRelease response = obs_data_create();
    obs_data_set_array(response, "types", idsArray);
    req->SendOKResponse(response);
}

/**
 * Get settings of the specified source
 *
 * @param {String} `sourceName` Name of the source item.
 * @param {String (optional) `sourceType` Type of the specified source. Useful for type-checking if you expect a specific settings schema.
 *
 * @return {String} `sourceName` Source name
 * @return {String} `sourceType` Type of the specified source
 * @return {Object} `sourceSettings` Source settings. Varying between source types.
 *
 * @api requests
 * @name GetSourceSettings
 * @category sources
 * @since unreleased
 */
 void WSRequestHandler::HandleGetSourceSettings(WSRequestHandler* req) {
    if (!req->hasField("sourceName")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    const char* sourceName = obs_data_get_string(req->data, "sourceName");
    OBSSourceAutoRelease source = obs_get_source_by_name(sourceName);
    if (!source) {
        req->SendErrorResponse("specified source doesn't exist");
        return;
    }

    if (req->hasField("sourceType")) {
        QString actualSourceType = obs_source_get_id(source);
        QString requestedType = obs_data_get_string(req->data, "sourceType");

        if (actualSourceType != requestedType) {
            req->SendErrorResponse("specified source exists but is not of expected type");
            return;
        }
    }

    OBSDataAutoRelease sourceSettings = obs_source_get_settings(source);

    OBSDataAutoRelease response = obs_data_create();
    obs_data_set_string(response, "sourceName", obs_source_get_name(source));
    obs_data_set_string(response, "sourceType", obs_source_get_id(source));
    obs_data_set_obj(response, "sourceSettings", sourceSettings);
    req->SendOKResponse(response);
}

/**
 * Set settings of the specified source.
 *
 * @param {String} `sourceName` Name of the source item.
 * @param {String (optional)} `sourceType` Type of the specified source. Useful for type-checking to avoid settings a set of settings incompatible with the actual source's type.
 * @param {Object} `sourceSettings` Source settings. Varying between source types.
 *
 * @return {String} `sourceName` Source name
 * @return {String} `sourceType` Type of the specified source
 * @return {Object} `sourceSettings` Source settings. Varying between source types.
 *
 * @api requests
 * @name SetSourceSettings
 * @category sources
 * @since unreleased
 */
void WSRequestHandler::HandleSetSourceSettings(WSRequestHandler* req) {
    if (!req->hasField("sourceName") || !req->hasField("sourceSettings")) {
        req->SendErrorResponse("missing request parameters");
        return;
    }

    const char* sourceName = obs_data_get_string(req->data, "sourceName");
    OBSSourceAutoRelease source = obs_get_source_by_name(sourceName);
    if (!source) {
        req->SendErrorResponse("specified source doesn't exist");
        return;
    }

    if (req->hasField("sourceType")) {
        QString actualSourceType = obs_source_get_id(source);
        QString requestedType = obs_data_get_string(req->data, "sourceType");

        if (actualSourceType != requestedType) {
            req->SendErrorResponse("specified source exists but is not of expected type");
            return;
        }
    }

    OBSDataAutoRelease currentSettings = obs_source_get_settings(source);
    OBSDataAutoRelease newSettings = obs_data_get_obj(req->data, "sourceSettings");

    OBSDataAutoRelease sourceSettings = obs_data_create();
    obs_data_apply(sourceSettings, currentSettings);
    obs_data_apply(sourceSettings, newSettings);

    obs_source_update(source, sourceSettings);
    obs_source_update_properties(source);

    OBSDataAutoRelease response = obs_data_create();
    obs_data_set_string(response, "sourceName", obs_source_get_name(source));
    obs_data_set_string(response, "sourceType", obs_source_get_id(source));
    obs_data_set_obj(response, "sourceSettings", sourceSettings);
    req->SendOKResponse(response);
}

/**
 * Get configured special sources like Desktop Audio and Mic/Aux sources.
 *
 * @return {String (optional)} `desktop-1` Name of the first Desktop Audio capture source.
 * @return {String (optional)} `desktop-2` Name of the second Desktop Audio capture source.
 * @return {String (optional)} `mic-1` Name of the first Mic/Aux input source.
 * @return {String (optional)} `mic-2` Name of the second Mic/Aux input source.
 * @return {String (optional)} `mic-3` NAme of the third Mic/Aux input source.
 *
 * @api requests
 * @name GetSpecialSources
 * @category sources
 * @since 4.1.0
 */
 void WSRequestHandler::HandleGetSpecialSources(WSRequestHandler* req) {
    OBSDataAutoRelease response = obs_data_create();

    QMap<const char*, int> sources;
    sources["desktop-1"] = 1;
    sources["desktop-2"] = 2;
    sources["mic-1"] = 3;
    sources["mic-2"] = 4;
    sources["mic-3"] = 5;

    QMapIterator<const char*, int> i(sources);
    while (i.hasNext()) {
        i.next();

        const char* id = i.key();
        OBSSourceAutoRelease source = obs_get_output_source(i.value());
        if (source) {
            obs_data_set_string(response, id, obs_source_get_name(source));
        }
    }

    req->SendOKResponse(response);
}