## ORBextractor.h

//#include <opencv/cv.h>
#include <opencv2/imgproc.hpp>

## optimizer.h
M
    // void static OptimizeEssentialGraph(Map* pMap, KeyFrame* pLoopKF, KeyFrame* pCurKF,
    //                                    const LoopClosing::KeyFrameAndPose &NonCorrectedSim3,
    //                                    const LoopClosing::KeyFrameAndPose &CorrectedSim3,
    //                                    const map<KeyFrame *, set<KeyFrame *> > &LoopConnections,
    //                                    const bool &bFixScale);

## LocalMapping.h

A - 39 
class Atlas;

M     
LocalMapping(Map* pMap, const float bMonocular, Atlas* pAtlas);

A - 126
Atlas* mpAtlas;

## loopclosing.h
注释所有

## PnPSolver.h
A 
#include <opencv2/core/types_c.h>
