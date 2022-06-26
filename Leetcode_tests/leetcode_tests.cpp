//
// Created by bysou on 2022/4/24.
//

#include "gtest/gtest.h"

/**
 * 868. 二进制间距
 给定一个正整数 n，找到并返回 n 的二进制表示中两个 相邻 1 之间的 最长距离 。如果不存在两个相邻的 1，返回 0 。
 如果只有 0 将两个 1 分隔开（可能不存在 0 ），则认为这两个 1 彼此 相邻 。两个 1 之间的距离是它们的二进制表示中位置的绝对差。例如，"1001" 中的两个 1 的距离为 3 。

 */

using namespace std;
TEST(bit_spacing, test) {
  auto f = [](int n) -> int {
    int index_start = -1;
    int index_end = -1;

    int cnt = 0;
    //  按位与之后求最长, 位数
    for (int i = 0; i < 32; ++i) {
      //  第i位=1
      if ((n >> i) & 1) {
        index_start = index_end;
        index_end = i;

        if (index_start == -1) continue;

        cnt = (index_end - index_start) > cnt ? (index_end - index_start) : cnt;

      }
    }
    return cnt;
  };

  cout << f(5) << (1 >> 1);
}

/**
398. 随机数索引
给你一个可能含有 重复元素 的整数数组nums ，请你随机输出给定的目标数字target 的索引。你可以假设给定的数字一定存在于数组中。

实现 Solution 类：

Solution(int[] nums) 用数组 nums 初始化对象。
int pick(int target) 从 nums 中选出一个满足 nums[i] == target 的随机索引 i 。如果存在多个有效的索引，则每个索引的返回概率应当相等。

 * **/

#include <map>
TEST(random_index, test) {
  class Solution {
   public:
    Solution(vector<int> &nums) {
      for (int i = 0; i < nums.size(); ++i) {
        save[nums[i]].push_back(i);
      }
    }

    map<int, vector<int>> save;
    int pick(int target) {
      return save[target][rand() % (save[target].size())];

    }
  };
  vector<int> vec{1, 2, 3, 3, 3};
  Solution solution(vec);
  cout << solution.pick(3) << "\n";
  cout << solution.pick(1) << "\n";
  cout << solution.pick(3) << "\n";
  cout << solution.pick(3) << "\n";
  cout << solution.pick(3) << "\n";

}

/**
 * 1. 两数之和
给定一个整数数组 nums和一个整数目标值 target，请你在该数组中找出 和为目标值 target 的那两个整数，并返回它们的数组下标。

你可以假设每种输入只会对应一个答案。但是，数组中同一个元素在答案里不能重复出现。

你可以按任意顺序返回答案。

 输入：nums = [2,7,11,15], target = 9
输出：[0,1]
解释：因为 nums[0] + nums[1] == 9 ，返回 [0, 1] 。

 * **/

TEST(sum, test) {
  class Solution {
   public:
    vector<int> twoSum(vector<int> nums, int target) {

      if (nums.size() == 2) {
        return {0, 1};
      }

      for (int i = 0; i < nums.size() - 1; ++i) {
        for (int j = i + 1; j < nums.size(); ++j) {
          if (nums[i] + nums[j] == target) {
            return {i, j};
          }
        }
      }
      return {};
    }
  };

  Solution s;
  vector<int> res = s.twoSum({3, 2, 4}, 6);
  cout << res[0] << " " << res[1];
}

/**
 * 883. 三维形体投影面积
在n x n的网格grid中，我们放置了一些与 x，y，z 三轴对齐的1 x 1 x 1立方体。

每个值v = grid[i][j]表示 v个正方体叠放在单元格(i, j)上。

现在，我们查看这些立方体在 xy、yz和 zx平面上的投影。

投影就像影子，将 三维 形体映射到一个 二维 平面上。从顶部、前面和侧面看立方体时，我们会看到“影子”。

返回 所有三个投影的总面积 。


 * **/

TEST(volume_projection_area, test) {
  class Solution {
   public:
    int projectionArea(vector<vector<int>> grid) {
      int x = 0;
      int y = 0;
      int z = 0;
      vector<int> temp_y;
      temp_y.resize(grid.size());

      for (int i = 0; i < grid.size(); ++i) {
        int temp_x = 0;
        for (int j = 0; j < grid[0].size(); ++j) {
          //  x
          temp_x = max(temp_x, grid[i][j]);

          //  y
          temp_y[j] = max(temp_y[j], grid[i][j]);
          //  z
          if (grid[i][j] != 0) {
            z += 1;
          }
        }

        x += temp_x;
//        temp_y = 0;
      }
      for (auto &it : temp_y) {
        y += it;
      }
      return (x + y + z);

    }
  };

  Solution s;
  cout << s.projectionArea({{1, 0}, {0, 2}});
}

/**
 * 9. 回文数
给你一个整数 x ，如果 x 是一个回文整数，返回 true ；否则，返回 false 。

回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整数。

例如，121 是回文，而 123 不是。

 * **/


#include <stack>
TEST(number_of_palindromes, test) {
  class Solution {
   public:
    bool isPalindrome(int x) {
      if (x < 0) {
        return false;
      }
      std::string nums(std::to_string(x));

      for (int i = 0; i < nums.size() / 2; ++i) {
        if (nums.at(i) == nums.at(nums.size() - i - 1)) {
          return false;
        }
      }
      return true;
      std::stack<char> st_;
      for (auto &item : nums) {
        st_.emplace(item);
      }
      for (int i = 0; i < nums.size() / 2; ++i) {
        if (st_.top() == nums.at(i)) {
          st_.pop();
        } else {
          return false;
        }
      }
      return true;
    }
  };

  Solution s;
  cout << s.isPalindrome(112211);
}

/**
 * 13. 罗马数字转整数
罗马数字包含以下七种字符: I， V， X， L，C，D 和 M。

字符          数值
I             1
V             5
X             10
L             50
C             100
D             500
M             1000
例如， 罗马数字 2 写做 II ，即为两个并列的 1 。12 写做 XII ，即为 X + II 。 27 写做 XXVII, 即为 XX + V + II 。

通常情况下，罗马数字中小的数字在大的数字的右边。但也存在特例，例如 4 不写做 IIII，而是 IV。数字 1 在数字 5 的左边，所表示的数等于大数 5 减小数 1 得到的数值 4 。同样地，数字 9 表示为 IX。这个特殊的规则只适用于以下六种情况：

I 可以放在 V (5) 和 X (10) 的左边，来表示 4 和 9。
X 可以放在 L (50) 和 C (100) 的左边，来表示 40 和 90。 
C 可以放在 D (500) 和 M (1000) 的左边，来表示 400 和 900。
给定一个罗马数字，将其转换成整数。
 * **/

#include <unordered_map>
TEST(roman_number, test) {
  class Solution {
   public:
    int romanToInt(string s) {

      int cnt = 0;
      std::unordered_map<int, char> map;
      int index;

      for (int i = 0; i < s.size(); ++i) {
        map[i] = s.at(i);
      }

      for (auto &itr : map) {
        if (itr.second == 'M')
          index = itr.first;
        cnt += 1000;
      }
      for (auto &itr : map) {
        if (itr.second == 'D' && index > itr.first) {
          cnt += 500;
        } else {
          index = itr.first;
          cnt -= 500;
        };
      }
      for (auto &itr : map) {
        if (itr.second == 'C' && index > itr.first) {
          cnt += 500;
        } else {
          index = itr.first;
          cnt -= 500;
        };
      }
      for (auto &itr : map) {
        if (itr.second == 'L' && index > itr.first) {
          cnt += 500;
        } else {
          index = itr.first;
          cnt -= 500;
        };
      }
      for (auto &itr : map) {
        if (itr.second == 'X' && index > itr.first) {
          cnt += 500;
        } else {
          index = itr.first;
          cnt -= 500;
        };
      }
      for (auto &itr : map) {
        if (itr.second == 'V' && index > itr.first) {
          cnt += 500;
        } else {
          index = itr.first;
          cnt -= 500;
        };
      }
      for (auto &itr : map) {
        if (itr.second == 'I' && index > itr.first) {
          cnt += 500;
        } else {
          index = itr.first;
          cnt -= 500;
        };

      }

      return cnt;
    }
  };

  Solution s;
  cout << s.romanToInt("LVIII");
}

/**
 * 14. 最长公共前缀

 编写一个函数来查找字符串数组中的最长公共前缀。

如果不存在公共前缀，返回空字符串""。


示例 1：

输入：strs = ["flower","flow","flight"]
输出："fl"
示例 2：

输入：strs = ["dog","racecar","car"]
输出：""
解释：输入不存在公共前缀。


 * **/
TEST(longest_common_prefix, test){
  class Solution {
   public:
    string longestCommonPrefix(vector<string> strs) {
      if(strs.size() == 0){
        return {};
      }
      if (strs.size() == 1){
        return strs.at(0);
      }

      int cnt = strs.at(0).size();

      if (cnt == 0){
        return {strs.at(0)};
      }

      for(int i = 1; i < strs.size(); ++i){
        if(strs.at(i).size() < cnt){
          cnt = strs.at(i).size();
          min_index = i;
        }
      }

      for(int i = 0; i <strs.at(min_index).size(); ++i){
        comm_str.push_back(strs.at(min_index).at(i));
        for(int j = 0 ; j < strs.size(); ++j){
          if(strs.at(j).at(i) == comm_str.at(i) ){

          }else{
//            break;
            comm_str.at(i)='\0';
            comm_str.pop_back();
            return {comm_str.data()};
          }
        }
      }

    }

    vector<char> comm_str;

    int min_index = 0;
    int min_cnt = 0;
  };

  Solution s;
  cout << s.longestCommonPrefix({"a"});

}
/**
 * 26. 删除有序数组中的重复项
 * 输入：nums = [0,0,1,1,1,2,2,3,3,4]
    输出：5, nums = [0,1,2,3,4]
 * **/
#include "io_units.h"
TEST(set_func, test) {
  class Solution {
   public:
    int removeDuplicates(vector<int> &nums) {
      if (nums.empty()) {
        return 0;
      }
      if (nums.size() == 1) {
        return 1;
      }
      vector<int> tmp{nums.at(0)};
      int index = 0;
      for (int i = 1; i < nums.size(); ++i) {
        if (tmp.at(index) != nums.at(i)) {
          tmp.emplace_back(nums.at(i));
          index++;
        }
      }
      nums = std::move(tmp);
      return nums.size();

    }
  };

  Solution s;
  vector<int> zzz{0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
  cout << s.removeDuplicates(zzz);
  PRINT_INT_ARRAY(zzz.data(), zzz.size());
}

/**
 * 27. 移除元素
 * 输入：nums = [3,2,2,3], val = 3
    输出：2, nums = [2,2]
 * **/
TEST(remove_target_func, test) {
  class Solution {
   public:
    int removeElement(vector<int> &nums, int val) {
//      for (int i = 0; i < nums.size(); ++i) {
//        if (nums.at(i) == val) {
//          nums.erase(nums.begin() + i);
//        }
      auto it = remove(nums.begin(),nums.end(), val);
      nums.erase(it, nums.end());
      return nums.size();

      }


  };

  Solution s;
  vector<int>nums{0,1,2,2,3,0,4,2};
  cout << s.removeElement(nums,2) << "\n";
  PRINT_INT_ARRAY(nums.data(), nums.size());

}

/**
 * 28. 实现 strStr()
 * 输入：haystack = "hello", needle = "ll"
    输出：2
 * **/
TEST(strstr, test) {
  class Solution {
   public:
    int strStr(string haystack, string needle) {
      if (haystack.size() == 0 || needle.size() == 0 ){
        return 0;
      }
      if (haystack.size() < needle.size()){
        return -1;
      }

      int i = 0;
      while ( !(i==haystack.size()) ){
        if (haystack.at(i) == needle.at(0)){
          int k = i;
          if (needle.size() == 1){
            return i;
          }
          if (i + needle.size() > haystack.size()){
            break;
          }
          for (int j = 1; j < needle.size(); ++j) {

            if (haystack.at(i+j)!=needle.at(j)){
//              i = i+j;
              i = k + 1;
              break;

            }
            if (j == (needle.size() -1)){
              return i;
            }
          }
        } else{
          i++;
        }

      }
      return -1;
  };


  };

  Solution s;
  cout << s.strStr("a","a") << "\n";

}
