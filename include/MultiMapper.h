/** 
 * File:   MultiMapper.h
 * Author: hdaoud
 *
 * Created on November 15, 2016, 12:15 PM
 */

#ifndef MULTIMAPPER_H
#define	MULTIMAPPER_H

//#include "MapPoint.h"
//#include "KeyFrame.h"
#include <string>
#include "System.h"
#include "Map.h"
#include "Frame.h"
#include "KeyFrameDatabase.h"
#include "ORBVocabulary.h"
#include "ORBextractor.h"
#include "LoopClosing.h"
#include "LocalMapping.h"
#include "Viewer.h"
#include "MapDrawer.h"
#include "FrameDrawer.h"
#include "Converter.h"
#include <iomanip>

#include <set>
#include <mutex>


namespace ORB_SLAM2
{

class Map;
class LoopClosing;
class LocalMapping;
class KeyFrameDatabase;

class MultiMapper
{
public:
    typedef pair<Map*, KeyFrameDatabase*> MapAndKFDB;
    typedef map<KeyFrame*,g2o::Sim3,std::less<KeyFrame*>,
        Eigen::aligned_allocator<std::pair<const KeyFrame*, g2o::Sim3> > > KeyFrameAndPose;
    
public:
    MultiMapper();
    MultiMapper(Map* pMap, KeyFrameDatabase* pDB, ORBVocabulary* pVoc);
    
    // Main function
    void Run();
    
    bool InitFromFile(const string &dirName);
    void AddMap(Map* mpMap, KeyFrameDatabase* mpKeyFrameDB); // Add completed map to the MultiMapper
    void EraseMap(Map* mpMap);// Remove specific Map from the MultiMapper
    int RequestMapUpdate(); // Request update of uncompleted maps from all robots/maps 
    void RegisterMap(int mpID); //Register map/robot in the MultiMapper
    void UpdatePosesAndAdd(Map* pMap, Map* pMapBase, const g2o::Sim3 g2oScw, KeyFrame* pKFini);//Update Poses of Keyframes in the map where one of its keyframes matches a keyframe in the main map
    
    void SetLoopCloser(LoopClosing* pLoopCloser);
//    LoopClosing* GetLoopCloser();
    
    void SetTracker(Tracking* pTracker);

    void SetLocalMapper(LocalMapping* pLocalMapper);
    
    void RequestReset();
    void RequestPause();
    bool isPaused();
    void Continue();
    void RequestFinish();    
    bool isFinished();
    
    void SaveTrajectory(const string &filename);
    void SaveMapTrajectory(Map* pMap, const string& filename);
    
    std::vector<Map*> GetAllMaps();
    
    void clear();
    
    vector<Map*> mvpMapOrigins;
    std::mutex mMutexMuliMapUpdate;
    
    bool mbUpdatingMapPoses;
    
protected:
    
    bool CheckNewMap();

    bool DetectLoop();

    bool ComputeSim3();
    
    void SearchAndFuse(const KeyFrameAndPose &CorrectedPosesMap, Map* pMap);
    
    void MergeMaps(Map* pMap, Map* pMapBase);
    
    void ResetIfRequested();
    bool mbResetRequested;
    std::mutex mMutexReset;
    
    Map* mpMap;
    LocalMapping* mpLocalMapper;
    Tracking* mpTracker;
    
    LoopClosing* mpLoopCloser;
    std::thread* mptLoopClosing;
    
    std::vector<Map*> mvpMaps;
    std::vector<KeyFrameDatabase*> mvpKFDB;
    std::vector<MapAndKFDB> mvMapAndKFDB;
    long unsigned int mnMaxMapId;
    
    bool Pause();
    bool CheckFinish();
    void SetFinish();
    bool mbPaused;
    bool mbFinishRequested;
    bool mbFinished;
    bool mbPauseRequested;
    bool mbContinue;
    std::mutex mMutexPause;
    std::mutex mMutexFinish;
    
    std::mutex mMutexLoopQueue;
    
    bool mbMatchedBefore;
    bool mbSwapped;
    cv::Mat Tcc;
    KeyFrame* mpMatchedKF;
    cv::Mat mScw;
    g2o::Sim3 mg2oScw;
    std::vector<MapPoint*> mvpCurrentMatchedPoints;
    std::vector<MapPoint*> mvpLoopMapPoints;
    
    //BoW
    KeyFrameDatabase* mpKeyFrameDB;
    ORBVocabulary* mpORBVocabulary;
    
    
    //System* mpiORBSystem;
    
    std::mutex mMutexMultiMapper;
};

}
#endif	/* MULTIMAPPER_H */

