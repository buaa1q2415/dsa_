#include <vector>
#include <algorithm>
#include <type_traits>

template <typename T>
class Sort{
private:
    Sort()=default;
    ~Sort()=default;

    template <typename Compare=std::less<T>>
    static int BinarySearch(const std::vector<T>& arr,const T& target,int left,int right,Compare cmp){
        while(left<=right){
            int mid=left+(right-left)/2;
            if(cmp(target,arr[mid])){
                right=mid-1;
            }else{
                left=mid+1;
            }
        }
        return left;
    }

    template <typename Compare=std::less<T>>
    static void Merge(std::vector<T> &arr,int left,int mid,int right,Compare cmp,std::vector<T> &temp){
        int i=left,j=mid+1,k=0;
        while(i<=mid&&j<=right){
            if(cmp(arr[i],arr[j])) temp[k++]=arr[i++];
            else temp[k++]=arr[j++];
        }
        while(i<=mid) temp[k++]=arr[i++];
        while(j<=right) temp[k++]=arr[j++];
        for(int i=0;i<k;i++) arr[left+i]=temp[i];
    }

    template <typename Compare=std::less<T>>
    static void MergeSortCore(std::vector<T> &arr,int left,int right,Compare cmp,std::vector<T> &temp){
        if(left<right){
            int mid=left+(right-left)/2;
            MergeSortCore(arr,left,mid,cmp,temp);
            MergeSortCore(arr,mid+1,right,cmp,temp);
            Merge(arr,left,mid,right,cmp,temp);
        }
    }

    template <typename Compare=std::less<T>>
    static void Heapify(std::vector<T>& arr,int left,int right,int index,Compare cmp){
        int flag=index,l=2*index-left+1,r=2*index-left+2;
        if(l<right&&cmp(arr[l],arr[flag])) flag=l;
        if(r<right&&cmp(arr[r],arr[flag])) flag=r;
        if(flag!=index){
            std::swap(arr[index],arr[flag]);
            Heapify(arr,left,right,flag,cmp);
        }
    }

    template <typename Compare=std::less<T>>
    static void BuildHeap(std::vector<T>& arr,int left,int right,Compare cmp){
        for(int i=left+(right-left+1)/2-1; i>=left; --i){
            Heapify(arr,left,right,i,cmp);
        }
    }

    template <typename Compare=std::less<T>>
    static std::pair<int,int> ThreeWayPartition(std::vector<T>& arr,int left,int right,T x,Compare cmp){
        int i=left,lbound=left,rbound=right;
        while(i<=rbound){
            if(cmp(arr[i],x)){
                std::swap(arr[i],arr[lbound]);
                lbound++,i++;
            }else if(cmp(x,arr[i])){
                std::swap(arr[i],arr[rbound]);
                rbound--;
            }else{
                i++;
            }
        }
        return std::make_pair(lbound,rbound);
    }

public:
    /* 选择排序：每次从[i,right]中选择一个最小的元素，放到i位置，然后在[i+1,right]继续
     * 时间复杂度：O(n^2)
     * 空间复杂度：O(1)
     * 稳定性：不稳定
     */
    template <typename Compare=std::less<T>>
    static void SelectSort(std::vector<T>& arr,int left,int right,Compare cmp){
        for(int i = left; i <= right; ++i){
            int edge = i;
            for(int j=i+1; j<=right; ++j){
                if(cmp(arr[j],arr[edge])){
                    edge = j;
                }
            }
            if(edge != i) std::swap(arr[i],arr[edge]);
        }
    }

    /* 插入排序：[left,i]已经有序，将i+1插入向下滑动到有序位置，然后继续
     * 时间复杂度：O(n^2)（二分查找优化比较次数）
     * 空间复杂度：O(1)
     * 稳定性：稳定
     */
    template <typename Compare=std::less<T>>
    static void InsertSort(std::vector<T>& arr,int left,int right,Compare cmp){
        for(int i=left+1;i<=right;i++){
            int j=i-1;
            T toinsert=arr[i];
            int insertpos=BinarySearch(arr,toinsert,left,j,cmp);
            while(j>=insertpos){
                arr[j+1]=arr[j];
                j--;
            }
            arr[j+1]=toinsert;
        }
    }

    /* 冒泡排序：每次从[left,right-i]中相邻位置较大的元素冒上去，放到right-i位置，然后在[left,right-i-1]继续
     * 时间复杂度：O(n^2)
     * 空间复杂度：O(1)
     * 稳定性：稳定
     */
    template <typename Compare=std::less<T>>
    static void BubbleSort(std::vector<T>& arr,int left,int right,Compare cmp){
        for(int i=0;i<right-left;i++){
            int flag=0;
            for(int j=left;j<=right-i-1;j++){
                if(cmp(arr[j+1],arr[j])){
                    std::swap(arr[j],arr[j+1]);
                    flag=1;
                }
            }
            if(!flag) break;
        }
    }

    /* 归并排序：分治法，将[left,right]分成两半，分别排序后合并
     * 时间复杂度：O(nlogn)
     * 空间复杂度：O(n)
     * 稳定性：稳定
     */
    template <typename Compare=std::less<T>>
    static void MergeSort(std::vector<T>& arr,int left,int right,Compare cmp){
        std::vector<T> temp(right-left+1);
        MergeSortCore(arr,left,right,cmp,temp);
    }

    /* 堆排序：利用堆这种数据结构进行排序
     * 时间复杂度：O(nlogn)
     * 空间复杂度：O(1)
     * 稳定性：不稳定
     */
    template <typename Compare=std::less<T>>
    static void HeapSort(std::vector<T> &arr,int left,int right,Compare cmp){
        BuildHeap(arr,left,right,cmp);
        for(int i=right; i>left; --i){
            std::swap(arr[left],arr[i]);
            Heapify(arr,left,i-1,left,cmp);
        }
    }

    /* 快速排序：分治法，将[left,right]根据某个基准值划分成三部分，小于基准值、等于基准值、大于基准值，分别排序
     * 时间复杂度：O(nlogn)平均，O(n^2)最坏
     * 空间复杂度：O(logn)
     * 稳定性：不稳定
     */
    template <typename Compare=std::less<T>>
    static void QuickSort(std::vector<T>& arr,int left,int right,Compare cmp){//三路分区
        if(left>=right) return;
        int mid=left+(right-left)/2;
        if(cmp(arr[mid],arr[left])) std::swap(arr[left],arr[mid]);
        if(cmp(arr[right],arr[left])) std::swap(arr[left],arr[right]);
        if(cmp(arr[right],arr[mid])) std::swap(arr[mid],arr[right]);
        T value=arr[mid];  //三数取中
        std::pair<int,int> p=ThreeWayPartition(arr,left,right,value,cmp);
        QuickSort(arr,left,p.first-1,cmp);
        QuickSort(arr,p.second+1,right,cmp);
    }

    /* 希尔排序：基于插入排序的一种优化，先将数组按一定间隔分组，对每组进行插入排序，逐步缩小间隔直到为1
     * 时间复杂度：O(n^(3/2))~O(n^(7/6))，具体取决于增量序列
     * 空间复杂度：O(1)
     * 稳定性：不稳定
     */
    template <typename Compare=std::less<T>>
    static void ShellSort(std::vector<T>& arr, int left, int right, Compare cmp) {
        int len = right - left + 1;
        if (len <= 1) return;
        // 计算 Knuth 增量序列的最大间隔
        int h = 1;
        while (h < len / 3) {
            h = 3*h+1;
        }

        // 逐步缩小增量
        while (h >= 1) {
            // 对每个子序列进行插入排序
            for (int i = left + h; i <= right; i++) {
                T temp = arr[i];
                int j = i;
                while (j >= left+h && cmp(temp, arr[j-h])) {
                    arr[j] = arr[j-h];
                    j -= h;
                }
                arr[j] = temp;
            }
            h /= 3; // 缩小增量
        }
    }

    /* 计数排序：适用于整数且范围不大的数组，通过统计每个元素出现的次数来排序
     * 时间复杂度：O(n + k)，k为元素范围
     * 空间复杂度：O(k)，k为元素范围
     * 稳定性：稳定
     */
    template <typename Compare=std::less<T>>
    static void CountingSort(std::vector<T>& arr, int left, int right) {
        static_assert(std::is_integral_v<T>, "计数排序仅支持整数类型");
        if (left >= right) return;
        int minVal = *std::min_element(arr.begin() + left, arr.begin() + right + 1);
        int maxVal = *std::max_element(arr.begin() + left, arr.begin() + right + 1);
        int range = maxVal - minVal + 1;

        std::vector<T> count(range, 0);
        for (int i = left; i <= right; i++) {
            count[static_cast<int>(arr[i]-minVal)]++;
        }

        int index = left;
        for (int i = 0; i < range; i++) {
            while (count[i]-- > 0) {
                arr[index++] = static_cast<T>(i+minVal);
            }
        }
    }

    /* 基数排序：适用于整数，通过按位排序实现整体排序
     * 时间复杂度：O(d*(n + k))，d为数字位数
     * 空间复杂度：O(n + k)，k为元素范围
     * 稳定性：稳定
     */
    template <typename Compare=std::less<T>>
    static void RadixSort(std::vector<T>& arr, int left, int right) {
        /* 这是用隔板分隔造桶的基数排序实现，支持负数排序，cache友好
         * 一个较为简单的实现思路是利用链表桶，通过首尾指针+连接链表，不会像数组桶那样浪费大量内存空间，但cache不友好
         */
        static_assert(std::is_integral_v<T>, "基数排序仅支持整数类型");
        if (left >= right) return;


        using U = std::make_unsigned_t<T>;
        int n = right - left + 1;

        // 符号位翻转
        const T signMask = static_cast<T>(1) << (sizeof(T) * 8 - 1);
        for (int i = left; i <= right; ++i) {
            arr[i] ^= signMask;
        }

        // 寻找最大值
        U maxVal = 0;
        for (int i = left; i <= right; ++i) {
            if (static_cast<U>(arr[i]) > maxVal) maxVal = static_cast<U>(arr[i]);
        }

        std::vector<T> temp(n);

        // 基数排序主循环
        for (U exp = 1; maxVal / exp > 0; exp *= 10) {
            // 统计每个桶(0-9)里将会有多少个元素+前缀和
            int count[10] = {0};
            for (int i = left; i <= right; ++i) {
                int digit = (static_cast<U>(arr[i]) / exp) % 10;
                count[digit]++;
            }
            for (int i = 1; i < 10; ++i) {
                count[i] += count[i - 1];
            }

            // 放置
            for (int i = right; i >= left; --i) {
                int digit = (static_cast<U>(arr[i]) / exp) % 10;
                
                // count[digit]-1 是当前元素在 temp 中的位置（相对 0 索引）
                // 放入 temp
                temp[count[digit] - 1] = arr[i];
                
                // 边界左移
                count[digit]--;
            }

            // 拷贝回原数组
            for (int i = 0; i < n; ++i) {
                arr[left + i] = temp[i];
            }
        }

        // 恢复符号位
        for (int i = left; i <= right; ++i) {
            arr[i] ^= signMask;
        }
    }

};
