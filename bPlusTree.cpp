#include <bits/stdc++.h>
using namespace std;

int init()
{
    cout << "Order of B Plus Tree : ";
    int order;
    cin >> order;
    cout << "Enter 1: Key Insert" << endl;
    cout << "Enter 2: Key Search" << endl;
    cout << "Enter 3: Bplus Tree Display" << endl;
    cout << "Enter 4: Exit !" << endl;
    return order;
}

class block
{
public:
    int numKeys;
    bool isLeaf;
    vector<int> keys;
    vector<block *> children;

    block *left;
    block *right;

    block()
    {
        left = right = NULL;
        numKeys = keys.size();
    }
    block(vector<int> keys, vector<block *> children, int numKeys, bool isLeaf)
    {
        this->keys = keys;
        this->children = children;
        this->numKeys = numKeys;
        this->isLeaf = isLeaf;
    }
};


class bPlusTree
{
    int order;
    block *root;

public:
    bPlusTree(int order)
    {
        this->root = NULL;
        this->order = order;
    }

    block *searchBlock(int value)
    {
        if (root == NULL)
        {
            return NULL;
        }
        else
        {
            block *cur = root, *par;
            while (!(cur->isLeaf))
            {
                par = cur;
                int flag = 1;
                for (int i = 0; i < cur->numKeys; i++)
                {
                    if (cur->keys[i] >= value)
                    {
                        flag = 0;
                        cur = cur->children[i];
                        break;
                    }
                }
                if (flag)
                    cur = cur->children[cur->numKeys];
            }
            return cur;
        }
    }

    block *findParent(block *cur, block *blk)
    {
        block *par = NULL;
        if (cur->isLeaf)
        {
            return NULL;
        }
        for (int i = 0; i <= cur->numKeys; i++)
        {
            if (cur->children[i] == blk)
            {
                return cur;
            }
            else
            {
                par = findParent(cur->children[i], blk);
                if (par != NULL)
                {
                    return par;
                }
            }
        }
        return par;
    }

    bool searchKey(int value)
    {
        block *blk = searchBlock(value);
        if (blk == NULL)
        {
            return 0;
        }
        for (int i = 0; i < blk->numKeys; i++)
        {
            if (value == blk->keys[i])
            {
                return 1;
            }
        }
        return 0;
    }

    void insertBlock(int value)
    {
        if (root == NULL)
        {
            root = new block({value}, {NULL, NULL}, 1, true);
        }
        else
        {
            block *cur = searchBlock(value);
            cur->keys.push_back(value);
            sort(cur->keys.begin(), cur->keys.end());
            cur->numKeys++;
            if (cur->numKeys <= order - 1)
            {
                cur->children.push_back(cur->children.back());
                cur->children[cur->numKeys - 1] = NULL;
            }
            else
            {
                block *blk = new block;
                block *rt = cur->right;

                if (rt != NULL)
                {
                    rt->left = blk;
                }
                blk->right = rt;
                blk->left = cur;
                cur->right = blk;

                blk->isLeaf = 1;
                vector<int> temp = cur->keys;

                int slice = (order) / 2;
                if (order % 2)
                    slice++;
                cur->keys.clear();
                cur->keys.insert(cur->keys.end(), temp.begin(), temp.begin() + slice);
                blk->keys.insert(blk->keys.end(), temp.begin() + slice, temp.end());
                blk->numKeys = blk->keys.size();
                cur->numKeys = cur->keys.size();
                while (blk->children.size() < blk->numKeys + 1)
                {
                    blk->children.push_back(NULL);
                }
                while (cur->children.size() > cur->numKeys + 1)
                {
                    cur->children.pop_back();
                }

                if (cur != root)
                {
                    insertInside(cur->keys.back(), findParent(root, cur), cur, blk);
                }
                else
                {
                    block *newRoot = new block({cur->keys.back()}, {cur, blk}, 1, false);
                    root = newRoot;
                }
            }
        }
    }

    void insertInside(int value, block *blk, block *left, block *right)
    {
        if (blk->numKeys + 1 < order)
        {
            int i = 0;
            while (value > blk->keys[i] && i < blk->numKeys)
            {
                i++;
            }
            blk->keys.push_back(0);
            blk->children.push_back(NULL);
            for (int j = blk->numKeys; j > i; j--)
            {
                blk->children[j + 1] = blk->children[j];
                blk->keys[j] = blk->keys[j - 1];
            }
            blk->children[i + 1] = right;
            blk->keys[i] = value;
            blk->numKeys = blk->keys.size();
        }
        else
        {
            block *newBlock = new block;
            vector<int> tempKeys = blk->keys;
            vector<block *> tempChildren = blk->children;
            tempKeys.push_back(0);
            tempChildren.push_back(NULL);

            int i = 0;
            while (value > blk->keys[i] && i < blk->numKeys)
            {
                i++;
            }
            for (int j = order - 1; j > i; j--)
            {
                tempKeys[j] = tempKeys[j - 1];
                tempChildren[j + 1] = tempChildren[j];
            }
            tempKeys[i] = value;
            tempChildren[i + 1] = right;
            newBlock->isLeaf = 0;
            blk->numKeys = (order) / 2;
            blk->keys = tempKeys;
            blk->children = tempChildren;
            newBlock->numKeys = order - 1 - blk->numKeys;
            while (blk->children.size() - 1 > blk->numKeys)
            {
                blk->children.pop_back();
                blk->keys.pop_back();
            }
            int valShift = tempKeys[blk->numKeys];
            for (int i = 0, j = blk->numKeys + 1; j < tempKeys.size(); i++, j++)
            {
                newBlock->keys.push_back(tempKeys[j]);
                newBlock->children.push_back(tempChildren[j]);
            }
            newBlock->children.push_back(tempChildren.back());

            if (blk != root)
            {
                insertInside(valShift, findParent(root, blk), blk, newBlock);
            }
            else
            {
                block *newBlock2 = new block({valShift}, {blk, newBlock}, 1, 0);
                root = newBlock2;
            }
        }
    }
    void printBlock(block *blk)
    {
        if (blk == NULL)
        {
            return;
        }
        for (int i = 0; i < blk->keys.size(); i++)
        {
            if (i != blk->keys.size() - 1)
            {
                cout << blk->keys[i] << ',';
            }
            else
                cout << blk->keys[i];
        }
    }
    void display(block *root)
    {
        queue<block *> q;
        q.push(root);
        if (root == NULL)
        {
            cout << "Empty Tree ! !";
            return;
        }
        while (q.size())
        {
            int sz = q.size();
            while (sz)
            {
                block *x = q.front();
                q.pop();
                sz--;
                if (x == NULL)
                {
                    continue;
                }
                printBlock(x);
                for (int i = 0; i < x->numKeys + 1; i++)
                {
                    q.push(x->children[i]);
                }
                cout << "  ";
            }
            cout << endl;
        }
        cout << endl;
    }

    block *getRoot()
    {
        return root;
    }
};

int main()
{
    int order = init();
    bPlusTree tree(order);
    int flag, val;
    while (1)
    {
        cin >> flag;
        if (flag == 1)
        {
            cout << "Enter Insert Value: ";
            cin >> val;
            if (tree.searchKey(val))
            {
                cout << "Key Already Exists ! !" << endl;
            }
            else
                tree.insertBlock(val);
            continue;
        }
        if (flag == 2)
        {
            cout << "Enter Search Value: ";
            int val;
            cin >> val;
            if (!tree.searchKey(val))
            {
                cout << "Key Doesnt Exist in tree !" << endl;
            }
            else
            {
                cout << "Key Exists in Tree !" << endl;
            }
            continue;
        }
        if (flag == 3)
        {
            tree.display(tree.getRoot());
            continue;
        }
        if (flag == 4)
        {
            cout << endl
                 << "Exiting..." << endl;
            break;
        }
        cout << endl
             << "Invalid Input !" << endl;
    }
    return 0;
}