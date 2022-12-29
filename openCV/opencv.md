## cv::mat 中的 nan
```C++
// 定位 nan
cv::Mat mask = cv::Mat(mat != mat);

// 修改 nan
cv::patchNaNs(mat1, 128);

// 检查 nan、inf 或 out of range
// the function either returns false (when quiet=true) or throws an exception.
bool ret = cv::checkRange(mat);
```