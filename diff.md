# 新增文件
##  BlockDatabase.h
    vector<KFBlockTable*> mpKFBlockDB;
    vector<MPBlockTable*> mpMPBlockDB;
记录每个数据块的信息

## Atlas.h
    - 存放临时关键帧块
        - vector<KFBlockPos> mKeyFrameBlockTempID;// 为了方便查找
        - vector<KeyFrameBlock*> mKeyFrameBlockTemp;
    - 等待存储关键帧块
        - vector<KFBlockPos> mKeyFrameBlockWaitingID;// 为了方便查找
        - vector<KeyFrameBlock*> mKeyFrameBlockWaiting;
    - Load 反序列化KF块
        - KeyFrameBlock* mKeyFrameBlockLoading;
    - 当前地图中存在的关键帧块
        - set<string> mCurrExistKFBlocksID;
        - set<KeyFrameBlock*> mCurrExistKFBlocks;
    - 当前所在的关键帧块
        - KFBlockPos mCurrKFBlockPos;
    - 当前位置周围的关键帧块ID
        - vector<string> mvKFBIdAroundCurrPos;
    - 等待检查的关键帧
        - std::list<KeyFrame*> mlKeyFramesWaitCheck;
    - 标记删除的地图点块（调试用）
        - vector<KeyFrame*> vpDeleteKFs;

    - CheckKeyFrameSave()
        

## KeyFrameBlock.h
- struct KFBlockPos  块的中心坐标
- struct KFBlockTable 数据快的坐标，id，创建时间
    - KFBlockPos mKFBlockPos;

- class KeyFrameBlock    Block的保存、加载；
    - KFBlockPos mKFBlockPos;
    - KFBlockTable mKFBlockTable；
    - BlockDatabase* mBlockDB;
    - std::set<KeyFrame*> mspKeyFrames;

    - **ComputeMPBlock()** 计算关键帧块观测到的地图点块
        1. 获取KF观测到的地图点
        2. 计算地图点在哪个地图点块的范围內
        3. 建立关键帧块与地图点块的对应关系

## MapPointBlock.h
- struct MPBlockPos
- struct MPBlockTable
    - MPBlockPos MPB_pos;

- struct MPBlockTable
- class MapPointBlock
    - MPBlockPos mMPBlockPos;
    - MPBlockTable mMPBlockTable;
    - std::set<MapPoint*> mspMapPoints;//all MP in Map block 




# 修改ORB_SLAM2文件
## KeyFrame.h

- 45 typedef int KFBID[3];
- 64 ~KeyFrame();


  KeyFrame* GetMinIDCovisibleKeyFrames();
  void SetKFBlockID(int KFB_x, int KFB_y, int KFB_z);

  void SetDelete(bool flag);
  bool GetDelete();


  KFBID mKFBId;
  int KFBlock_scale;
  bool isSaved;
  bool deleted;
  std::mutex mMutexDel;


  std::map<long unsigned int, id_map> mmMapPoints_nId;
  std::map<long unsigned int, int> mConnectedKeyFrameWeights_nId;
  long unsigned int mpParent_nId;
  std::set<long unsigned int> mmChildrens_nId;
  std::map<long unsigned int, id_map> mmLoopEdges_nId;


LocalMaping.h
class Atlas;
LocalMaping{
    LocalMapping(Map* pMap, const float bMonocular, Atlas* pAtlas);
    Atlas* mpAtlas;
    }

## Map.h

#include "KeyFrameBlock.h"


class KeyFrameBlock;
class MapPointBlock;


    //获取地图中具体关键帧
    KeyFrame* GetKeyFrame(long unsigned int KFId);
    //获取地图中具体地图点
    MapPoint* GetMapPoint(long unsigned int MPId);
    // 将KFB块中的KF添加到map中
    void AddKeyFrameFromKFBlock(KeyFrameBlock* pKFB);
    // 将MPB块中的MP添加到map中
    void AddMapPointFromMPBlock(MapPointBlock* pMPB);
    //删除已经存储的关键帧
    void DeleteKeyFrames(vector<KeyFrame*> vpKFs);
    //删除已经存储的地图点
    void DeleteMapPoints(vector<MapPoint*> vpMPs);


    void SetDelKFID(unsigned long int KFID);
    void SetDelMPID(unsigned long int MPID);
    unsigned long int GetDelKFID();
    unsigned long int GetDelMPID();


        unsigned long int DeleteKFID;
    unsigned long int DeleteMPID;


## MapPoint.h

typedef int MPBID[3];


    ~MapPoint();

    static bool cmp(MapPoint* pMP1, MapPoint* pMP2)
    {
      return pMP1->mnId < pMP2->mnId;
    }

        void SetBadFlag(bool flag = false);


  void SetMPBlockID(int MPB_x, int MPB_y, int MPB_z);

  void UpdateObservations();


      bool isSaved;
    MPBID mMPBId;
    int MPBlock_scale;


## Optimize.h
   
    // void static OptimizeEssentialGraph(Map* pMap, KeyFrame* pLoopKF, KeyFrame* pCurKF,
    //                                    const LoopClosing::KeyFrameAndPose &NonCorrectedSim3,
    //                                    const LoopClosing::KeyFrameAndPose &CorrectedSim3,
    //                                    const map<KeyFrame *, set<KeyFrame *> > &LoopConnections,
    //                                    const bool &bFixScale);

## ORBExtractor

#include <opencv2/imgproc.hpp>


## PnPsolver
#include <opencv2/core/types_c.h>


##  system.h
#include "Atlas.h"

class Atlas;
class BlockDatabase;

    BlockDatabase* mpBlockDatabase;
    Atlas* mpAtlas;
    std::thread* mptAtlas;

## tracking.h
#include <opencv2/imgproc/types_c.h>

    void SetAtlas(Atlas* pAtlas);


    std::mutex mMutexLocalMP;

    
    void PrintLocalKF()
    {
        for(vector<KeyFrame*>::iterator it = mvpLocalKeyFrames.begin(); it != mvpLocalKeyFrames.end(); it++)
        {
            cout << "Track Local KF :" << (*it)->mnId << endl;
        }
    }

    void SetlocalMinKFId(long unsigned int id)
    {
        unique_lock<mutex> lock(mMutexLocalMinKFId);
        mLocalMinKFId = id;
    }
    long unsigned int GetlocalMinKFId()
    {
        unique_lock<mutex> lock(mMutexLocalMinKFId);
        return mLocalMinKFId;
    }

    void SetlocalMinMPId(long unsigned int id)
    {
        unique_lock<mutex> lock(mMutexLocalMinMPId);
        mLocalMinMPId = id;
    }
    long unsigned int GetlocalMinMPId()
    {
        unique_lock<mutex> lock(mMutexLocalMinMPId);
        return mLocalMinMPId;
    }

        Atlas* mpAtlas;


            std::mutex mMutexLocalMinMPId;
    std::mutex mMutexLocalMinKFId;
    long unsigned int mLocalMinMPId;
    long unsigned int mLocalMinKFId;


==============================================================================

## KeyFrameDatabase.cc
            list<KeyFrame*> &lKFs = mvInvertedFile[vit->first];

## KeyFrame.cc
...

## map.cc
...

## mapPoint.cc
...

## ORBextractor.cc
if(bestDist<=TH_HIGH && (RemoveRepeat.insert(pMP->mnId).second))


## system.cc

	mpBlockDatabase = new BlockDatabase();
	mpAtlas = new Atlas(strSettingsFile, mpMap, mpKeyFrameDatabase, mpVocabulary, bReuse, mpBlockDatabase);//bReuse = false


      mpTracker->SetLocalMapper(mpLocalMapper);
//    mpTracker->SetLoopClosing(mpLoopCloser);

    mpTracker->SetAtlas(mpAtlas);

    mpLocalMapper->SetTracker(mpTracker);
//    mpLocalMapper->SetLoopCloser(mpLoopCloser);

//    mpLoopCloser->SetTracker(mpTracker);
//    mpLoopCloser->SetLocalMapper(mpLocalMapper);

    mpAtlas->SetLocalMapper(mpLocalMapper);
    mpAtlas->SetTracker(mpTracker);

    mptAtlas = new thread(&ORB_SLAM2::Atlas::Run, mpAtlas);

  ....

## tracking.cc
bool Tracking::TrackLocalMap()
{
    unique_lock<mutex> lock(mMutexLocalMP);
}


## 