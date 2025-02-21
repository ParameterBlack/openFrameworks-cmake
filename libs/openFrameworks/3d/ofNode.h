
#pragma once

#include "ofVectorMath.h"
#include "of3dUtils.h"
#include "ofAppRunner.h"
#include "ofParameter.h"
#include <array>


/// \brief A generic 3d object in space with transformation (position, rotation, scale).

class ofNode {
public:
	/// \cond INTERNAL
	
	ofNode();
	virtual ~ofNode();
	ofNode(const ofNode & node);
	ofNode(ofNode && node);
	ofNode & operator=(const ofNode & node);
	ofNode & operator=(ofNode && node);

	/// \endcond

	/// \name Parent Node
	/// \{

	/// \brief Set parent for the node. The node will inherit transformations from parent.
	/// \param param0 Reference to the ofNode which becomes the parent node.
	/// \param param1 Boolean if maintain child's global transformations (default = false). 
	void setParent(ofNode& parent, bool bMaintainGlobalTransform = false);

	/// \brief Remove parent node linking.
	/// \param param0 Boolean if maintain child's global transformations (default = false).
	void clearParent(bool bMaintainGlobalTransform = false);

	/// \brief Get the parent node of this node.
	/// \returns Pointer to parent ofNode.
	ofNode* getParent() const;

	/// \}
	/// \name Getters
	/// \{
	
	/// \brief Get node's local position as a 3D vector.
	/// \returns A 3D vector with the local coordinates.
	ofVec3f getPosition() const;

	/// \brief Get node's local x position. 
	/// \returns Local x coordinate as a float.
	float getX() const;

	/// \brief Get node's local y position.
	/// \returns Local y coordinate as a float.
	float getY() const;

	/// \brief Get node's local z position.
	/// \returns Local z coordinate as a float.
	float getZ() const;
	
	/// \brief Get the node's local x axis as 3d vector.
	/// \returns A normalized 3D vector of the node's local x axis direction.
	ofVec3f getXAxis() const;

	/// \brief Get the node's local y axis as 3d vector.
	/// \returns A normalized 3D vector of the node's local y axis direction.
	ofVec3f getYAxis() const;

	/// \brief Get the node's local z axis as 3d vector.
	/// \returns A normalized 3D vector of the node's local z axis direction.
	ofVec3f getZAxis() const;
	
	/// \brief Get direction of node's side aka local x axis, as 3d vector.
	/// \returns A normalized 3D vector of the node's local x axis direction.
	ofVec3f getSideDir() const;
	
	/// \brief Get direction the node looks at aka local -z axis, as 3d vector.
	/// \returns A normalized 3D vector of the node's local -z axis direction.
	ofVec3f getLookAtDir()const;

	/// \brief Get direction of node's top aka local y axis, as 3d vector.
	/// \returns A normalized 3D vector of the node's local y axis direction.
	ofVec3f getUpDir() const;
	
	/// \brief Get pitch of node, aka the rotation along local x axis.
	/// \returns The rotation around the local x axis in degrees, as a float.
	float getPitch() const;
	
	/// \brief Get heading of node, aka the rotation along local y axis.
	/// \returns The rotation around the local y axis in degrees, as a float.
	float getHeading() const;
	
	/// \brief Get roll of node, aka the rotation along local z axis.
	/// \returns The rotation around the local z axis in degrees, as a float.
	float getRoll() const;
	
	/// \brief Get the local orientation of the node as a quaternion.
	/// \returns An ofQuaternion of local orientation (useful for complex rotations)
	ofQuaternion getOrientationQuat() const;
	
	/// \brief Get local orientation of node in degrees around x, y, and z axes.
	/// \returns The local x, y and z axes orientation in degrees, as a 3D vector.
	ofVec3f getOrientationEuler() const;
	
	/// \brief Get local scale of node in xyz axes where 1 is default.
	/// \returns The local scale in the xyz axes where 1 = 100% of size.
	ofVec3f getScale() const;

	/// \brief Get node's local transformations (position, orientation, scale).
	/// \returns A refrence to ofMatrix4x4 containing node's local transformations.
	/// \sa https://open.gl/transformations
	const ofMatrix4x4& getLocalTransformMatrix() const;
	
	// TODO: optimize and cache these
	// (parent would need to know about its children so it can inform them 
	// to update their global matrices if it itself transforms)
	
	/// \brief Get node's global transformations (position, orientation, scale).
	/// \returns A refrence to ofMatrix4x4 containing node's global transformations.
	/// \sa https://open.gl/transformations
	ofMatrix4x4 getGlobalTransformMatrix() const;
	
	/// \brief Get node's global position as a 3D vector.
	/// \returns A 3D vector with the global coordinates.
	ofVec3f getGlobalPosition() const;
	
	/// \brief Get the global orientation of the node as a quaternion.
	/// \returns An ofQuaternion of the global orientations(useful for complex rotations)
	ofQuaternion getGlobalOrientation() const;
	
	/// \brief Get global scale of node in xyz axes where 1 is default.
	/// \returns The global scale in the xyz axes where 1 = 100% of size.
	ofVec3f getGlobalScale() const;

	/// \}	
	/// \name Setters
	/// \{

	/// \brief Directly set the local transformation matrix.
	/// \param param0 Desired transform matrix as ref to ofMatrix4x4. 
	void setTransformMatrix(const ofMatrix4x4 &m44);
	
	/// \brief Set the local position of the node using xyz coordinates.
	/// \param param0 Desired local x coordinate as a float.
	/// \param param1 Desired local y coordinate as a float.
	/// \param param2 Desired local z coordinate as a float.
	void setPosition(float px, float py, float pz);
	
	/// \brief Set the local position of the node using a 3D vector of coordinates.
	/// \param param0 Desired local xyz coordinates as ref to 3D vector.	
	void setPosition(const ofVec3f& p);
	
	/// \brief Set the global position of the node using xyz coordinates.
	/// \param param0 Desired global x coordinate as a float.
	/// \param param1 Desired global y coordinate as a float.
	/// \param param2 Desired global z coordinate as a float.	
	void setGlobalPosition(float px, float py, float pz);

	/// \brief Set the global position of the node using a 3D vector of coordinates.
	/// \param param0 Desired global xyz coordinates as ref to 3D vector.		
	void setGlobalPosition(const ofVec3f& p);

	/// \brief Set local orientation with a quaternion.
	/// \param param0 Desired local orientation as ref to an ofQuaternion.
	void setOrientation(const ofQuaternion& q);
	
	/// \brief Set local orientation with xyz euler angles.
	/// \param param0 Desired local xyz angles in degrees, as ref to 3D vector. 
	/// \note Using euler angles can cause gimbal lock.
	/// \sa https://en.wikipedia.org/wiki/Gimbal_lock
	void setOrientation(const ofVec3f& eulerAngles);
	
	/// \brief Set global orientation with a quaternion.
	/// \param param0 Desired global orientation as ref to an ofQuaternion.	
	void setGlobalOrientation(const ofQuaternion& q);

	/// \brief Set local uniform scale (x, y, and z are equally scaled).
	/// \param param0 Desired scale for all axes as a float where 1 = 100%.
	void setScale(float s);
	
	/// \brief Set local scale for xyz axes individually.
	/// \param param0 Desired local scale for x axis as a float where 1 = 100%.
	/// \param param1 Desired local scale for y axis as a float where 1 = 100%.
	/// \param param2 Desired local scale for z axis as a float where 1 = 100%.
	void setScale(float sx, float sy, float sz);
	
	/// \brief Set local scale for xyz axes individually with a 3D vector.
	/// \param param0 Desired local scale for all axes as ref to 3D vector where 1 = 100%.
	void setScale(const ofVec3f& s);
	
	/// \}
	/// \name Modifiers
	/// \{

	/// \brief Move node by relative amount with xyz coordinates.
	/// \param param0 Desired relative position change along x axis as a float.
	/// \param param1 Desired relative position change along y axis as a float.
	/// \param param2 Desired relative position change along z axis as a float.
	void move(float x, float y, float z);			
	
	/// \brief Move node by relative amount with xyz as ref to 3D vector.
	/// \param param0 Desired relative position change along all axes as ref to 3D vector.
	void move(const ofVec3f& offset);	

	/// \brief Move node left+right relative to current position (in local x axis).
	/// \param param0 Desired relative position change along local x axis as float.
	void truck(float amount);

	/// \brief Move node up+down relative to current position (in local y axis).
	/// \param param0 Desired relative position change along local y axis as float.
	void boom(float amount);

	/// \brief Move node backward+forward relative to current position (in local z axis).
	/// \param param0 Desired relative position change along local z axis as float.					
	void dolly(float amount);						
	
	/// \brief Tilt up+down relative to current orientation (around local x axis).
	/// \param param0 Desired relative rotation change along local x axis in degrees as float.
	void tilt(float degrees);						
	
	/// \brief Rotate left+right relative to current orientation (around local y axis).
	/// \param param0 Desired relative rotation change along local y axis in degrees as float.
	void pan(float degrees);

	/// \brief Roll left+right relative to current orientation (around local z axis).
	/// \param param0 Desired relative rotation change along local z axis in degrees as float.
	void roll(float degrees);					
	
	/// \brief Rotate relative to current orientation by quaternion.
	/// \param param0 Desired relative rotation change as a ref to ofQuaternion.	
	void rotate(const ofQuaternion& q);	
	
	/// \brief Rotate relative to current orientation around arbitrary axis.
	/// \param param0 Desired relative angle change in degrees as float.	
	/// \param param1 Desired axis to rotate around as a ref to cartesian 3D Vector.	
	void rotate(float degrees, const ofVec3f& v);

	/// \brief Rotate relative to current orientation around arbitrary axis.
	/// \param param0 Desired relative angle change in degrees as float.	
	/// \param param1 X angle of the axis to rotate around in degrees as float.
	/// \param param2 Y angle of the axis to rotate around in degrees as float.
	/// \param param3 Z angle of the axis to rotate around in degrees as float.
	void rotate(float degrees, float vx, float vy, float vz);
	
	/// \brief Rotate relative to current orientation by quaternion around point.
	/// \param param0 Desired relative rotation change as a ref to ofQuaternion.
	/// \param param1 Point to rotate around in local xyz coordinates as ref to 3D vector.	
	void rotateAround(const ofQuaternion& q, const ofVec3f& point);
	
	/// \brief Rotate relative to current orientation around arbitrary axis around point.
	/// \param param0 Desired relative angle change in degrees as float.    
	/// \param param1 The arbitrary axis to rotate around as ref to cartesian 3D vector.
    /// \param param2 Point to rotate around in local xyz coordinates as ref to 3D vector.
    void rotateAround(float degrees, const ofVec3f& axis, const ofVec3f& point);

    /// \brief Orient node to look at point (-z axis pointing to global position).
    /// \param param0 XYZ coordinates of point to look at as ref to 3D vector.
    /// \note This version calculates the up vector automatically to try to keep
    /// 	  it relatively consistant with the original angle.
    void lookAt(const ofVec3f& lookAtPosition);

    /// \brief Orient node to look at point (-z axis pointing to global position).
    /// \param param0 XYZ coordinates of point to look at as ref to 3D vector.
    /// \param param1 The desired up axis as a cartesian 3D vector.
    void lookAt(const ofVec3f& lookAtPosition, ofVec3f upVector);

    /// \brief Orient node to look at another node (-z axis pointing at other node).
    /// \param param0 A reference to the node to look at.
    /// \note This version calculates the up vector automatically to try to keep
    /// 	  it relatively consistant with the original angle.
    void lookAt(const ofNode& lookAtNode);
	
    /// \brief Orient node to look at another node (-z axis pointing at other node).
    /// \param param0 A reference to the node to look at.
    /// \param param1 The desired up axis as a ref to cartesian 3D vector.
    void lookAt(const ofNode& lookAtNode, const ofVec3f& upVector);
	
	/// \brief Orbit node around a global position at a specific radius.
	/// \param param0 The longitudinal position of the node in degrees as float.
	/// \param param1 The latitudinal position of the node in degrees as float.
	/// \param param2 The desired radius from the position in degrees as float.
	/// \param param3 The global position to orbit around as ref to 3D vector. Default = (0, 0, 0).	
	void orbit(float longitude, float latitude, float radius, const ofVec3f& centerPoint = ofVec3f(0, 0, 0));
	/// \brief Orbit node around another node at a specific radius.
	/// \param param0 The longitudinal position of the node in degrees as float.
	/// \param param1 The latitudinal position of the node in degrees as float.
	/// \param param2 The desired radius from the other node in degrees as float.
	/// \param param3 A reference to the node to rotate around.	
	void orbit(float longitude, float latitude, float radius, ofNode& centerNode);
	
	/// \}
	/// \name OpenGL Transformation
	/// \{
	
	/// \brief Set opengl renderer's modelview matrix to this nodes transform.
	/// \param param0 A pointer to the renderer you want to set to this node's transform;
	/// \note If you want to draw something at the position+orientation+scale of this node,
	/// call ofNode::transform(); write your draw code, and ofNode::restoreTransform();
	/// OR A simpler way is to extend ofNode and override ofNode::customDraw();
	void transformGL(ofBaseRenderer * renderer = nullptr) const;
	
	/// \brief Restore opengl renderer's previous modelview transform matrix.
	/// \param param0 A pointer to the renderer you want to restore transformation to;	
	void restoreTransformGL(ofBaseRenderer * renderer = nullptr) const;
	
	
	/// \brief Reset this node's transformations, position, rotation and scale.
	void resetTransform();
	
	/// \}
	/// \name Drawing
	/// \{

	/// \brief If you extend ofNode and wish to change the way it draws, extend this.
	/// \param param0 A pointer to the renderer you want to draw to.
	/// \note Try to not use global functions for rendering and instead use the passed
	/// renderer.
	virtual void customDraw(const ofBaseRenderer * renderer) const;
	
	/// \brief If you extend ofNode and wish to change the way it draws, extend this.
	/// \note Try to not use global functions for rendering and instead use the passed
	/// renderer.
	virtual void customDraw();

	
	/// \brief Draw the node as a white cube with xyz axes. 
	/// \note do NOT override this. 
	/// It transforms the node to its position+orientation+scale
	/// and calls the virtual 'customDraw' method above which you CAN override.
	virtual void draw() const;

	/// \}
	
protected:
	void createMatrix();
	void updateAxis();
	
	/// \brief classes extending ofNode can override this method to get
	/// notified when the position changed.
	virtual void onPositionChanged() {}

	/// \brief classes extending ofNode can override this methods to get notified when the orientation changed.
	virtual void onOrientationChanged() {}

	/// \brief classes extending ofNode can override this methods to get notified when the scale changed.
	virtual void onScaleChanged() {}

	ofNode * parent = nullptr;

private:
	void onParentPositionChanged(ofVec3f & position) {onPositionChanged();}
	void onParentOrientationChanged(ofQuaternion & orientation) {onOrientationChanged();}
	void onParentScaleChanged(ofVec3f & scale) {onScaleChanged();}

	ofParameter<ofVec3f> position;
	ofParameter<ofQuaternion> orientation;
	ofParameter<ofVec3f> scale;

	std::array<ofVec3f,3> axis;

	ofMatrix4x4 localTransformMatrix;
	bool legacyCustomDrawOverrided;
	std::set<ofNode*> children;

	void addListener(ofNode & node);
	void removeListener(ofNode & node);
//	ofMatrix4x4 globalTransformMatrix;
};
