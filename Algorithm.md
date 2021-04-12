## 2021.1.2 && 2021.1.3

### 1. [反转链表](https://leetcode-cn.com/problems/fan-zhuan-lian-biao-lcof/)
```C++
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
            ListNode *pre = NULL ; 
            while(head) {
                ListNode *tmp = head->next ; 
                head->next = pre ; 
                pre = head ; 
                head = tmp ; 
            }      
            return pre ; 
    }
};
```
### 2. [合并两个有序链表](https://leetcode-cn.com/problems/merge-two-sorted-lists/)

    注： 注意判断链表是否为空

```C++
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode *head ;
        if(l1 == NULL) return l2 ;
        if(l2 == NULL) return l1 ;  
        
        if(l1->val > l2->val ){
            head = l2 ; 
            l2 = l2->next ; 
        } else {
            head = l1 ; 
            l1 = l1->next ; 
        }

        ListNode *current = head ; 
        while(l1 && l2) {
            if(l1->val > l2->val){
                current->next = l2 ; 
                l2 = l2->next ;         
            }else {
                current->next = l1 ;
                l1 = l1->next ;  
            }
            current = current->next ; 
        }

        if(l1) current->next = l1 ; 
        else current->next = l2 ; 

        return head ; 

    }
};
```

### 3.strcpy 函数的实现

注： strcpy 本身也会存在覆盖的问题 ，好的解决方案是从高地址到低地址开始复制。

```C++

char* strcpy(char *des , const char *src){
    // 断言判断，为真则继续执行，为假则中断程序抛出异常，提高程序健壮性

    assert(des != NULL) ; 
    assert(src != NULL) ; 
    
    char *address = des ; // 返回首地址

    /*
    1. 大众做法，正确
    2. (*des++ = *src++) 而不是(des++ = src++)，因为地址是一个常量数，不是变量；先赋值，后地址后移。
    见：https://stackoverflow.com/questions/33375136/lvalue-required-as-left-operand-of-assignment-error-when-using-c
    
    while((*des++ = *src++) != '\0') ; 
    */

    /*
    1. 自己写的，特别注意 for 循环无法赋值 '\0' 给 des 做结尾，因为当 *(src + i ) == '\0' 就跳出了

    int i ; 
    for(i = 0 ; *(src + i ) != '\0' ; ++i) {
        *(des + i) = *(src + i) ; 
    }
    *(des + i) = '\0' ; 

    */
    return address ;
}
```

### 4.[链表中的倒数第k个节点](https://leetcode-cn.com/problems/lian-biao-zhong-dao-shu-di-kge-jie-dian-lcof/)

```C++
class Solution {
public:
    ListNode* getKthFromEnd(ListNode* head, int k) {
        ListNode *fast = head ; 
        ListNode *low = head ; 

        while(k--) { 
            if(fast == NULL) return NULL ; 
            fast = fast->next ; 
        }

        while(fast != NULL){
            low = low->next ; 
            fast = fast->next ; 
        }

        return low ; 
    }
};
```

### 5.[二叉树的镜像](https://leetcode-cn.com/problems/er-cha-shu-de-jing-xiang-lcof/)


```C++
class Solution {
public:
    TreeNode* mirrorTree(TreeNode* root) {

        if(root == NULL) {
            return NULL ; 
        }
        
        TreeNode *tmp = root->right ; 
        root->right = root->left ; 
        root->left = tmp ; 
        
        mirrorTree(root->right) ; 
        mirrorTree(root->left) ; 
        
        return root ; 
    }
};
```

### 6.[网格中的最短路径](https://leetcode-cn.com/problems/shortest-path-in-a-grid-with-obstacles-elimination/)

注：bfs 问题，注意 visit 需要是 3 维状态，不是以往简单的 2 维状态判断是否访问过。

```C++
class Solution {

struct node { 
    int x ; 
    int y ; 
    int k ; 
    int sum ; 
} ; 

public:
    int direction[4][2] = {{1,0} , {0,1} , {-1 ,0} , {0 , -1}} ; 
    // 右 ， 上 ， 左 ,  下

    int bfs(vector<vector<int>> &grid , int k ) {
        
        int rowSize = grid.size() ; 
        int colSize = grid[0].size() ; 

        if(rowSize ==  1 && colSize == 1) return 0 ; 
        if(k >= rowSize + colSize - 3) return rowSize + colSize - 2; // 有一组数据一直卡着超时，就是这种类型的特判
        k = min(k, rowSize + colSize - 3); // 对 k 的优化 , 即时间复杂度上的优化

        bool visit[rowSize][colSize][k+1] ; 
        memset(visit , false , sizeof(visit)) ; 
        queue<node> que ; 

        int ans = INT_MAX ; 
        node first ; first.x = 0 ; first.y = 0; first.k = k ; first.sum = 0 ; 
        que.push(first) ; 
        visit[0][0][k] = true ; 

        while(!que.empty()) {

            node tmp ; tmp = que.front() ; que.pop() ; 
            int x = tmp.x ; int y = tmp.y; 
            int k = tmp.k ; int sum = tmp.sum ;  

            if(x == rowSize - 1 && y == colSize - 1) {
                ans = min(ans , sum) ; 
                continue ; 
            }


            for(int i = 0 ; i < 4 ; ++i) {
                int tmp_x = x + direction[i][0] ; 
                int tmp_y = y + direction[i][1] ; 
                
                if(tmp_x >= 0 && tmp_x < rowSize && tmp_y >=0 && tmp_y < colSize && visit[tmp_x][tmp_y][k] == false ) {
                        
                        node tmp_node ;  tmp_node.x = tmp_x ; tmp_node.y = tmp_y ; tmp_node.k = k ; tmp_node.sum = sum + 1 ;  
                    // cout<<tmp_x<<" "<<tmp_y<<endl ; 
                        if (grid[tmp_x][tmp_y] == 0){
                            que.push(tmp_node) ; 
                            visit[tmp_x][tmp_y][tmp_node.k] = true ;
                        }
                        if(grid[tmp_x][tmp_y] == 1 && k >= 1) {
                            tmp_node.k = tmp_node.k - 1; 
                            que.push(tmp_node) ; 
                            visit[tmp_x][tmp_y][tmp_node.k] = true ;
                        }
                }
            }

        }
        return (ans == INT_MAX)? -1 : ans ; 
    }

    int shortestPath(vector<vector<int>>& grid, int k) {

        return bfs(grid , k) ; 

    }
};
```

7. [前 K 个高频元素](https://leetcode-cn.com/problems/top-k-frequent-elements/)

注：感觉不完全是 TOPK 问题，频率采用的是桶排序的算法，感觉比建立大小根堆，而采用优先的算法更切合题意要求的 O(nlogn) 时间复杂度。

```C++
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        
        // 统计出现个数
        unordered_map<int , int > Count_map ; 
        for(auto num : nums) {
            Count_map[num]++ ; 
        }

        //桶排序，将数字的频次 作为桶的索引index
        vector<vector<int> > Nums_vec(nums.size() + 1) ;
        int max_size = INT_MIN ; 
        for(auto iter = Count_map.begin() ; iter != Count_map.end() ; iter++){
            Nums_vec[iter->second].push_back(iter->first) ;
            max_size = max(max_size , iter->second) ; 
        }

        //从索引高到低依次遍历每个桶，如果桶不空，则取数直至取出前k个数
        int sum = 0 ;
        vector<int> ans ; 
        for(int i = max_size ; i>=0 ;i--){
            if(Nums_vec[i].size() != 0 && sum < k ){
                for(int j = 0 ; j < Nums_vec[i].size() ; j++){
                    if(sum < k ) ans.push_back(Nums_vec[i][j]) ; 
                    ++sum ; 
                }
            }
        }
        
        return ans; 
    }
};
```

## B 树的特征

![](https://user-gold-cdn.xitu.io/2019/1/6/168232b1eb401c01?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
1. 每个中间节点都包含 `k-1` 个元素和 `k` 个孩子 , 其中` m/2 <= k <= m ` 。
2. 每一个叶子节点都包含`k-1`个元素，其中` m/2 <= k <= m ` 。
3. 所有的叶子结点都位于同一层 。

B- 树主要应用于文件系统以及部分数据库索引，比如著名的非关系型数据库 MongoDB ;

## B+ 树的特征

![](https://pic2.zhimg.com/80/v2-d8b80c13d68de264b02e4cf9c3663dc9_720w.jpg)

1. 有 k 个子树的中间节点包含有 k 个元素（ B 树中是 k-1 个元素），每个元素不保存数据，只用来索引，所有数据都保存在叶子节点。
2. 所有的叶子结点中包含了全部元素的信息，及指向含这些元素记录的指针，且叶子结点本身依关键字的大小自小而大顺序链接。
3. 所有的中间节点元素都同时存在于子节点，在子节点元素中是最大（或最小）元素。

**B+树的优势**: 1.单一节点存储更多的元素，使得查询的 IO 次数更少 ; 2.所有查询都要查找到叶子节点，查询性能稳定; 3. 所有叶子节点形成有序链表，便于范围查询。

**总的原则，减少磁盘 IO 次数**