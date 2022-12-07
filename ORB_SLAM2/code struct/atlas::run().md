
# run()
  -  定位模式
    mbReuse = True
       1. 获取当前关键帧的相机中心
         mpTrack->mCurrentFrame.GetCameraCenter();
       2. 查询需要加载的相邻关键帧块的Id
         Atlas::CheckBlocksLoad()
          1. 根据Id从分块数据库中获取关键帧块的表头
              mBlockDatabase->GetKFBlockTable(vKFB_ID[i])
          2. 加载关键帧块对应的地图点块
              MapPointBlockLoad()
          3. 添加地图点
              - 向ORB map中添加地图点
                mpMap->AddMapPointFromMPBlock(mMapPointBlockLoading);
              - 记录已存在的Block
                Atlas::mCurrExistMPBlocks.insert(mMapPointBlockLoading);
          4. 加载关键帧块
            Atlas::KeyFrameBlockLoad()
            ifs >> Atlas::mKeyFrameBlockLoading
          5. 向ORB map中添加关键帧
              mpMap->AddKeyFrameFromKFBlock(mKeyFrameBlockLoading);
              map::mspKeyFrames.insert(pKF);
          6. 将地图点关联到对应的关键帧
             SetMapPoints()
          

  -  建图模式
        1. 检查关键帧队列（map中给入）
          CheckWaitKeyFrames()
          Atlas::mlKeyFramesWaitCheck
          该变量在localMapping文件中，在BA优化完成后，填充
          mpAtlas->AddKeyFrameToCheck(mpCurrentKeyFrame);
   
        2. 检查需要保存的关键帧
          Atlas::CheckKeyFrameSave()

             - ！stop
               bool stop = CheckFinish();
          
               1. 获得当前关键帧的共视关键帧中ID最小的关键帧
                 KeyFrame::GetMinIDCovisibleKeyFrames()
                  1. 获取共视关键帧
                    KeyFrame::GetVectorCovisibleKeyFrames()
                       return KeyFrame::mvpOrderedConnectedKeyFrames;
                  2. 计算共视关键帧的最小Id
                   
               2. 获取局部地图中的关键帧最小Id
                GetlocalMinKFId()  ??
                 return Tracking::mLocalMinKFId

               3. 选取GetlocalMinKFId与GetlocalMinKFId的最小Id
               
               4. **如果窗口内的关键帧，当前都没有参与计算，则将窗口内的关键帧Id都加入待删除列表**
                
              - stop
                  1. GetMaxKFid()
                      return mnMaxKFid;
                  2. 将剩余的关键帧分块保存
        
        3. 将待删除的关键帧将其添加到所属关键帧块
        
            1. 计算关键帧所属块的ID
              CalcKeyFrameBlockID()
            2. 设置KF的块坐标
             SetKFBlockID()
            3. 查找关键帧块在临时关键帧块列表中是否存在
              IsKeyFrameBlockTemp()
            4. 如果该KF所处的块还没有建立, 进行创建
              CreateKeyFremeBlock()
            5. AddKeyFrame()
              mspKeyFrames.insert(pKF);

        4. 将临时块和已经存储的块融合
          MergeMapPointBlock()
         
             1. 判断临时关地图点块是否存储过
               SelectMPBlock()
             2. 存储过，则将存储过的块加载，把两者融合
               MapPointBlockLoad(filename);
             3. 将保存的地图点合并到临时地图点
               mMapPointBlockTemp[i]->AddMapPoint((*vit));
             4. 将临时地图块加入代存储列表
             5. 清空临时块列表

        5. 将地图点块待存储列表中的地图点块序列化保存
          SaveMapPointBlock()
            
            1. 设置地图点块的信息表
            mMapPointBlockWaiting[i]->SetMPBlockTable(MPB_ID, 2022, 2022);
            1. 将block信息表添加到Block数据库
              mBlockDatabase->AddMPBlockTable(mMapPointBlockWaiting[i]->GetMPBlockTable());
            2. 将地图点以二进制的形式保存存到文件
              boost::archive::binary_oarchive oa(ofs, boost::archive::no_header);
              oa << mMapPointBlockWaiting[i];