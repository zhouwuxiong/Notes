Tracking::NeedNewKryFrame(){
1. 不插入关键帧
   - 定位模式
   - 正在进行回环矫正
   - 距离上次重定位的时间太近 && ？？map.KeyFrame.size > MaxFrame

2. 计算当前帧的价值
    - nRefMatches -> 参考帧中看到的地图点中，有多少地图点可以被至少3个关键帧观测到
    - bLocalMappingIdle -> LocalMaper是否空闲（在处理一个关键帧的过程中不接受插入新的关键帧）
    - bNeedToInsertClose -> 当前帧的特征点中，有少于100个特征点匹配到了当前存在的地图点（防止信息冗余）&& 有大于70个特征点没有与之对应的地图点（能提供足够的地图点信息）

3. 插入关键帧的条件
   - 距离上次插入很远(MaxFrame)
   - 距离上次插入有了一定时间（MinFrames）&& LocalMaper空闲 
   - 跟踪的效果很差 -> 当前帧看到的地图点 < 0.25*参考帧看到的地图点数量 && 当前帧满足作为参考帧的条件（bNeedToInsertClose）
   - （相比上一帧，当前帧看到的地图点数量明显减少 || 当前帧满足作为参考帧的条件） && 至少看到了15个地图点

4. LocalMaper是否空闲？如果不空闲则中断BA
}


Tracking::CreateNewKeyFrame(){
1.
}


Tracking::UpdateLocalKeyFrames{
1. 统计能观测到当前关键帧地图点的关键帧数量
2. 清空上一次跟踪的局部关键帧
3. 添加关键帧
   1. 策略1：当前帧的共视帧
   2. 策略2：当前帧的共视帧不够时，从共视帧中继续寻找局部关键帧
        - 策略1中局部关键帧的最佳共视10帧中，不在当前局部关键帧中的一个关键帧
        - 策略1中局部关键帧的一个子关键帧和父关键帧
}

Tracking::UpdateLocalPoints(){
1. 清空上一次跟踪的局部地图点
2. 将局部关键帧中的看到的所有地图点加入局部地图点
}

//判断MapPoint是否在当前帧的视野范围内
Frame::isInFrustum(){
1. MapPoint的世界坐标转为相机坐标(相机坐标系下的z轴是否大于0)
2. 相机坐标投影到相机平面（像素坐标是否超出图像大小）
3. MapPoint是否在相机能观测到的深度范围内（不同分辨率的图像观测范围不一样，高分辨率图像观测范围更远）
4. MapPoint是否在相机的观测角度内（相机光心与MapPoint的连线与MapPoint的平均观测角度的夹角小于60度）
5. 根据深度预测投影的特征点在图像金字塔的哪一层
}

//在局部地图中查找在当前帧视野范围内的点，将视野范围内的点和当前帧的特征点进行投影匹配
Tracking::SearchLocalPoints(){
1. 在LocalMapPoint中标记出当前帧已经看到的地图点，不需要进行投影匹配
2. 判断LocalMapPoint中每个MapPoint的投影是否在当前关键帧的视野范围内 -> isInFrustum
3. 对视野范围内的MapPoint通过投影进行特征点匹配 -> SearchByProjection
}
f
//计算平均观测方向以及观测距离范围
MapPoint::UpdateNormalAndDepth(){

}


TrackLocalMap(){
    步骤1：更新局部关键帧mvpLocalKeyFrames和局部地图点mvpLocalMapPoints
    Tracking::UpdateLocalMap(){
        Tracking::UpdateLocalKeyFrames();
        Tracking::UpdateLocalPoints();
    }
    步骤2：在局部地图中查找与当前帧匹配的MapPoints
    Tracking::SearchLocalPoints(){
        ...
        1. 判断MapPoint是否在当前帧的视野范围内
        isInFrustum()
        
        2. 对视野范围内的MapPoint通过投影进行特征点匹配
        SearchByProjection()
        
        3. 根据当前帧中的所有MapPoints（包含重投影生成的），进行位姿优化
        Optimizer::PoseOptimization(&mCurrentFrame);
        
        4：更新当前帧的MapPoints被观测程度，并统计跟踪局部地图的效果
        ...
        5. 判断是否跟踪成功
            是否有足够多的地图点被观测到（>30,如果刚刚进行重定位>50）
    }


}

Tracking::Track(){
1. 是否已经初始化 -> 初始化
   <SLAM模式>
2. 上一帧是否跟踪正常？ 否 -> 重定位
3. ..
4. 三种跟踪模式
5. 局部地图跟踪
6. 跟踪成功，插入关键帧？
7. 跟踪失败，系统重置？
8. 记录位姿信息
}


LocalMapping::Run(){

    ProcessNewKeyFrame()
}


LocalMapping::ProcessNewKeyFrame(){}
