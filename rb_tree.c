#include <stdlib.h>
#include <stdio.h>
//这里偷懒就应0代表黑色，1代表红色了
typedef struct RBTreeNode {
    int    data; //数据域
    int    color; //0黑色 1红色
    struct RBTreeNode *parent;//父亲结点
    struct RBTreeNode *left;  //左子结点
    struct RBTreeNode *right; //右子结点
} RBTreeNode;
struct RBTreeNode *LastRBTreeNode;
/**
 * 左旋
 *        parent                    parent
 *          8                         12
 *       4     12                  8     50  
 *           9    50      =>    4    9      70
 *                   70                      
 */
RBTreeNode *left_rotation(RBTreeNode *root)
{
    struct RBTreeNode *new_root;
    new_root         = root->right;
    root->right      = new_root->left;
    //将9的父亲设置为老的root 即8
    if (new_root->left != NULL)
    {
        new_root->left->parent = root;
    }
    //新root的parent即老parent
    new_root->parent = root->parent;
    //然后处理老root的parent
    if (root->parent == NULL)
    {
        //老root是根节点
        new_root->parent = NULL;
    }else{
        //判断父亲左右
        if (new_root->parent->left == root)
        {
            new_root->parent->left  = new_root;
        }else{
            new_root->parent->right = new_root;
        }
    }
    root->parent   = new_root;
    new_root->left = root;
    return new_root;
}
/**
 * 右旋
 *        parent                    parent
 *          8                  4
 *       4     12           2     8
 *    2    6         =>  1      6   12
 * 1                                
 */
RBTreeNode *right_rotation(RBTreeNode *root)
{
    struct RBTreeNode *new_root;
    new_root         = root->left;
    root->left       = new_root->right;
    
    //将6的父亲设置为老的root 即8
    if (new_root->right != NULL)
    {
        new_root->right->parent = root;
    }
    //新root的parent即老parent
    new_root->parent = root->parent;
    
    //然后处理老root的parent
    if (root->parent == NULL)
    {
        //老root是根节点
        new_root->parent = NULL;
    }else{
        //判断父亲左右
        if (new_root->parent->left == root)
        {
            new_root->parent->left = new_root;
        }else{
            new_root->parent->right= new_root;
        }
    }
    
    new_root->right = root;
    root->parent    = new_root;
    return new_root;
}
RBTreeNode *getNode(int data, RBTreeNode *parent)
{
    struct RBTreeNode *node;
    node = (struct RBTreeNode *)malloc(sizeof(struct RBTreeNode));
    node->data  = data;
    node->parent= parent;
    node->color = 1;
    node->right = NULL;
    node->left  = NULL;
    LastRBTreeNode = node;
    return node;
}

RBTreeNode *insert(RBTreeNode *root, int data, RBTreeNode *parent)
{
    if (NULL == root)
    {
        return getNode(data, parent);
    }
    int need_fix = 0;
    if (data >= root->data)   
    {
        root->right  = insert(root->right, data, root);
    }else{
        root->left   = insert(root->left, data, root);
    }
    return root;
}

RBTreeNode *rebalance(RBTreeNode *head, RBTreeNode *root)
{
    if (root->parent == NULL)
    {
        root->color = 0;
        return root;
    }
    //父亲不是NULL
    //如果父亲是黑色，该插入不破坏平衡性
    if (root->parent->color == 0)
    {
        return head;
    }
    //父亲是红色 一定有祖父，且祖父为黑色
    RBTreeNode *parent, *gparent, *uncle;
    parent = root->parent;
    gparent = root->parent->parent;
    if (parent == gparent->left)
    {
        uncle = gparent->right;
        //如果叔叔为红色, 不为NULL一定就是红色，不然不符合红黑树特性
        if (uncle != NULL){
            //父亲叔叔都置黑，祖父置红,对祖父递归自平衡
            
            gparent->color = 1;
            parent->color  = 0;
            uncle->color   = 0;
            return rebalance(head, gparent);
        }else{
        //如果叔叔为黑色，Nil(NULL)也是黑色哈
            //祖父置红，父亲置黑，对祖父右旋
            gparent->color = 1;
            parent->color  = 0;
            if (root == parent->right)
            {
                //1.root与父节点交换 并把父节点设为新root的左节点,即转化为1.1
                gparent->left = root;

                root->parent = gparent;
                root->left = parent;

                parent->parent = root;
                parent->right = NULL;
            }
            return gparent == head ? right_rotation(gparent) : head;
        }
    }else{
        uncle = gparent->left;
        //如果叔叔为红色, 不为NULL一定就是红色，不然不符合红黑树特性
        if (uncle != NULL){
            //父亲叔叔都置黑，祖父置红,对祖父递归自平衡
            gparent->color = 1;
            parent->color  = 0;
            uncle->color   = 0;
            return rebalance(head, gparent);
        }else{
        //如果叔叔为黑色，Nil(NULL)也是黑色哈
            //祖父置红，父亲置黑，对祖父右旋
            gparent->color = 1;
            parent->color  = 0;
            if (root == parent->left)
            {
                //1.root与父节点交换 并把父节点设为新root的左节点,即转化为2.1
                gparent->right = root;

                root->parent = gparent;
                root->right = parent;

                parent->parent = root;
                parent->left = NULL;
            }
            return gparent == head ? left_rotation(gparent) : head;
        }
    }
}
RBTreeNode *inserRB(RBTreeNode *root, int data, RBTreeNode *parent)
{
    root = insert(root,data,parent);
    root = rebalance(root,LastRBTreeNode);
    return root;
}
RBTreeNode *FindMin(RBTreeNode *root)
{
    if (NULL == root)
    {
        return NULL;
    }
    if (root->left == NULL)
    {
        return root;
    }else{
        return FindMin(root->left);
    }
}
RBTreeNode *FindMax(RBTreeNode *root)
{
    if (NULL == root)
    {
        return NULL;
    }
    if (root->right == NULL)
    {
        return root;
    }else{
        return FindMax(root->right);
    }
}
RBTreeNode *Delete(RBTreeNode *head, RBTreeNode *root, int target)
{
    if (NULL == root)
    {
        return NULL;
    }
    if (target > root->data){
        return Delete(head, root->right, target);
    }else if (target < root->data)
    {
        return Delete(head, root->left, target);
    }else{
    //相等的情况
        RBTreeNode *parent, *brother;
        parent = root->parent;
        int head_flag = parent == head;
        int red_flag  = root->color == 1;
        //左右节点都没
        if (root->left == NULL && root->right == NULL)
        {
            //特殊情况删除根节点
            if (parent == NULL)
            {
                return NULL;//删除自身
            }
            //被删除节点在左树上
            if (root == parent->left)
            {
                parent->left = NULL;
                free(parent->left);
                root = NULL;
                free(root);
                //被删除节点是红色，直接删除
                if (red_flag) return head;              
                //被删除节点是黑色,则必然存在兄弟节点,兄弟节点可能黑也可能红
                brother = parent->right;
                //1.兄弟节点没有左右节点
                if (brother->left == NULL && brother->right == NULL)
                {
                    brother->color = 1;
                    return head;
                }else if(brother->left == NULL && brother->right != NULL)
                //2.兄弟节点只有一个右节点 兄弟节点必然为黑色 此右节点必然为红色
                {
                    brother->right->color = parent->color;
                    parent = left_rotation(parent);
                    return head_flag ? parent : head;
                }else if(brother->left != NULL && brother->right == NULL)
                //3.兄弟节点只有一个左节点 兄弟节点必然为黑色 必然为红色
                {
                    //转化为场景2
                    parent->right        = brother->left;
                    //对于brother->left
                    brother->left->right = brother;
                    brother->left->color = 0;
                    brother->left->parent= parent;
                    //对于brother
                    brother->parent      = brother->left;
                    brother->color       = parent->color;
                    brother->left        = NULL;
                    parent = left_rotation(parent);
                    return head_flag ? parent : head;
                }else
                //4.兄弟节点左右节点都有
                {
                    //兄弟黑，必有俩红
                    if (brother->color == 0)
                    {
                        brother->right->color = parent->color;
                    }else{
                    //兄弟红，必有俩黑，且父亲必黑
                        brother->color       = 0;
                        brother->left->color = 1;
                    }
                    parent = left_rotation(parent);
                    return head_flag ? parent : head;
                }
            }else
            //被删除节点在右树上 跟左树是镜像关系
            {
                parent->right = NULL;
                free(parent->right);
                root = NULL;
                free(root);
                //被删除节点是红色，直接删除
                if (red_flag) return head;              
                //被删除节点是黑色,则必然存在黑色兄弟节点
                brother = parent->left;
                //1.兄弟节点没有左右节点
                if (brother->left == NULL && brother->right == NULL)
                {
                    brother->color = 1;
                    return head;
                }else if(brother->left != NULL && brother->right == NULL)
                //2.兄弟节点只有一个左节点
                {
                    brother->left->color = parent->color;
                    parent = right_rotation(parent);
                    return head_flag ? parent : head;
                }else if(brother->left == NULL && brother->right != NULL)
                //3.兄弟节点只有一个右节点
                {
                    //转化为场景2
                    parent->left         = brother->right;
                    //对于brother->right
                    brother->right->left = brother;
                    brother->right->color = 0;
                    brother->right->parent= parent;
                    //对于brother
                    brother->parent      = brother->right;
                    brother->color       = parent->color;
                    brother->right       = NULL;
                    parent = right_rotation(parent);
                    return head_flag ? parent : head;
                }else
                //4.兄弟节点左右节点都有
                {
                    //兄弟黑，必有俩红
                    if (brother->color == 0)
                    {
                        brother->left->color = parent->color;
                    }else{
                    //兄弟红，必有俩黑，且父亲必黑
                        brother->color       = 0;
                        brother->right->color = 1;
                    }
                    parent = right_rotation(parent);
                    return head_flag ? parent : head;
                }
            }
        }else if (root->left != NULL && root->right == NULL)
        //只有一个左节点 那么依据红黑树特性，root必然为黑色，且左节点为红色
        {
            root->left->color = 0;
            //被删除节点在左树上
            if (root == parent->left)
            {
                parent->left       = root->left;
                root->left->parent = parent;
            }else
            //被删除节点在右子树上
            {
                parent->right      = root->left;
                root->left->parent = parent;
            }
            root = NULL;
            free(root);
            return head;
        }else if (root->left == NULL && root->right != NULL)
        //只有一个右节点 那么依据红黑树特性，root必然为黑色，且右节点为红色
        {
            printf("root null:\n");
            root->right->color = 0;

            //被删除节点在左树上
            if (root == parent->left)
            {
                parent->left       = root->right;
                root->right->parent= parent;
            }else
            //被删除节点在右子树上
            {
                parent->right      = root->right;
                root->right->parent= parent;
            }
            root = NULL;
            free(root);
            return head;
        }else
        //左右节点都在，选一个前驱或者后继
        //后继节点：删除节点的右子树中的最小节点，即右子树中最左节点。
        //前驱节点：删除节点的左子树中最大节点，即左子树中最右节点。
        //这里使用后继节点作为代替结点
        {
            RBTreeNode *min = FindMin(root->right);
            root->data = min->data;
            return Delete(head, min, min->data);
        }
    }
    return head;
}
//前序遍历
void preOrderTraverse(RBTreeNode *root)
{
    if (root != NULL)
    {        
        if (root->parent != NULL)
        {
            printf("%d color: %d parent:%d\n", root->data, root->color, root->parent->data);
        }else{
            printf("%d color: %d\n", root->data, root->color);
        }
        preOrderTraverse(root->left);
        preOrderTraverse(root->right);
    }
}

int main(int argc, char const *argv[])
{
    struct RBTreeNode *node;

    //1.1 测试用例
    node = NULL;
    node = inserRB(node, 8, NULL);
    node = inserRB(node, 4, NULL);
    node = inserRB(node, 12, NULL);
    node = inserRB(node, 2, NULL);
    //node = inserRB(node, 20, NULL);
    //node = inserRB(node, 30, NULL);
    //node = inserRB(node, 14, NULL);
    //node = inser(node, 4, NULL);
    printf("***1测试用例 前序***:  \n");
    preOrderTraverse(node);

    //node = Delete(node,node,30);
    node = Delete(node,node,8);

    printf("***测试用例 前序***:  \n");
    preOrderTraverse(node);
    return 0;
}
