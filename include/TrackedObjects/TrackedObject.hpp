#pragma once

#include <iostream> // for standard I/O
#include <string>   // for strings
#include <sstream>  // string to number conversion
#include <opencv2/core.hpp>     // Basic OpenCV structures (Mat, Scalar)
#include <opencv2/viz.hpp>

class Camera;
struct PositionPacket;
class BoardViz2D;
struct CameraView;

using namespace std;
using namespace cv;

struct ArucoMarker
{
	float sideLength;
	int number;
	Affine3d Pose;

	static vector<Point3d> GetObjectPointsNoOffset(float SideLength);

	vector<Point3d> GetObjectPointsNoOffset();

	ArucoMarker()
		:sideLength(0.05),
		number(-1),
		Pose(Affine3d::Identity())
	{}

	ArucoMarker(float InSideLength, int InNumber)
		:sideLength(InSideLength),
		number(InNumber),
		Pose(Affine3d::Identity())
	{}

	ArucoMarker(float InSideLength, int InNumber, Affine3d InPose)
		:sideLength(InSideLength),
		number(InNumber),
		Pose(InPose)
	{}

	void DisplayMarker(viz::Viz3d* visualizer, Affine3d RootLocation, String rootName);
};

class TrackedObject
{
public:
	vector<ArucoMarker> markers;
	vector<TrackedObject*> childs;
	Affine3d Location;
	bool Unique;
	String Name;

public:

	TrackedObject()
	{};

	virtual Affine3d ResolveLocation(vector<Affine3d>& Cameras, vector<CameraView>& views);

	//Find the parameters and the accumulated transform of the tag in the component and it's childs
	virtual bool FindTag(int MarkerID, ArucoMarker& Marker, Affine3d& TransformToMarker);

	//Returns all the corners in 3D space of this object and it's childs, with the marker ID. Does not clear the array at start.
	virtual void GetObjectPoints(vector<vector<Point3d>>& MarkerCorners, vector<int>& MarkerIDs, Affine3d rootTransform = Affine3d::Identity(), vector<int> filter = {});

	//Given corners, solve this object's location using multiple tags at once
	virtual Affine3d GetObjectTransform(vector<vector<Point2f>> MarkerCorners, vector<int> MarkerIDs, Mat& CameraMatrix, Mat& DistanceCoefficients);

	virtual void DisplayRecursive2D(BoardViz2D visualizer, Affine3d RootLocation, String rootName);

	virtual void DisplayRecursive(viz::Viz3d* visualizer, Affine3d RootLocation, String rootName);

	virtual vector<PositionPacket> ToPacket(int BaseNumeral);
};



extern ArucoMarker center;

Affine3d GetTagTransform(float SideLength, std::vector<Point2f> Corners, Mat& CameraMatrix, Mat& DistanceCoefficients);

Affine3d GetTagTransform(float SideLength, std::vector<Point2f> Corners, Camera* Cam);

Affine3d GetTransformRelativeToTag(ArucoMarker& Tag, std::vector<Point2f> Corners, Camera* Cam);

void Tracker3DTest();