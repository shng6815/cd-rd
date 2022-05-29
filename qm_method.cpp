#include <algorithm>
#include <bitset>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string toBin(int a, int size) {  //이진수 변환
   string r;
   for (int i = size - 1; i >= 0; --i) {
      r = (a % 2 == 0 ? "0" : "1") + r;
      a /= 2;
   }
   return r;
}

int toDec(string n) {  // 십진수 변환
   int e = 1, sum = 0;
   for (int i = n.length() - 1; i >= 0; i--) {
      for (int j = 0; j < n.length() - i - 1; j++) {
         e *= 2;
      }
      if (n[i] == '1') {
         sum += e;
      }
      e = 1;
   }
   return sum;
}

void solve(vector<vector<vector<pair<string, bool>>>> &v, int n, int size) {
   int end = 0;
   vector<pair<string, bool>> v2;
   vector<vector<pair<string, bool>>> v3;
   int cnt;
   string temp;

   v.push_back(v3);
   for (int i = 0; i <= size; i++) {
      v[n + 1].push_back(v2);
   }

   for (int i = 0; i < v[n].size() - 1; i++) {
      for (int j = 0; j < v[n][i].size(); j++) {
         for (int k = 0; k < v[n][i + 1].size(); k++) {
            cnt = 0;
            for (int l = 0; l < size; l++) {
               if (v[n][i][j].first[l] != v[n][i + 1][k].first[l]) {
                  cnt++;
                  temp = v[n][i][j].first;
                  temp[l] = '-';
               }
            }
            if (cnt == 1) {
               end++;
               v[n][i][j].second = true;
               v[n][i + 1][k].second = true;
               v[n + 1][count(temp.begin(), temp.end(), '1')].push_back(
                   pair<string, bool>(temp, false));
            }
         }
      }
   }
   if (end) {
      solve(v, n + 1, size);
   } else {
      return;
   }
}

void countPi(vector<pair<int, int>> &v, string ans, int cnt,
             vector<bool> &map) {
   if (cnt == 0) {
      int temp = toDec(ans);
      for (int i = 0; i < v.size(); i++) {
         if (temp == v[i].first) {
            map[i] = true;
            v[i].second++;
            return;
         }
      }
      return;
   }
   if (ans[cnt - 1] == '-') {
      ans[cnt - 1] = '0';
      countPi(v, ans, cnt - 1, map);
      ans[cnt - 1] = '1';
      countPi(v, ans, cnt - 1, map);
   } else
      countPi(v, ans, cnt - 1, map);
}

void countEpi(vector<pair<int, int>> v, string ans, string ori,
              vector<string> &epi, int cnt) {
   if (cnt == 0) {
      int temp = toDec(ans);
      for (int i = 0; i < v.size(); i++) {
         if (temp == v[i].first) {
            if (v[i].second == 1) {
               epi.push_back(ori);
            }
            return;
         }
      }
      return;
   }
   if (ans[cnt - 1] == '-') {
      ans[cnt - 1] = '0';
      countEpi(v, ans, ori, epi, cnt - 1);
      ans[cnt - 1] = '1';
      countEpi(v, ans, ori, epi, cnt - 1);
   } else
      countEpi(v, ans, ori, epi, cnt - 1);
}

void deleteEpi(string ans, int cnt, vector<int> &minterm,
               vector<vector<bool>> &piMap) {
   if (cnt == 0) {
      int temp = toDec(ans);
      int idx = find(minterm.begin(), minterm.end(), temp) - minterm.begin();
      if (idx < minterm.size()) {
         minterm.erase(remove(minterm.begin(), minterm.end(), temp),
                       minterm.end());
         for (int i = 0; i < piMap.size(); i++) {
            piMap[i].erase(piMap[i].begin() + idx);
         }
      }

      return;
   }
   if (ans[cnt - 1] == '-') {
      ans[cnt - 1] = '0';
      deleteEpi(ans, cnt - 1, minterm, piMap);
      ans[cnt - 1] = '1';
      deleteEpi(ans, cnt - 1, minterm, piMap);
   } else
      deleteEpi(ans, cnt - 1, minterm, piMap);
}

void print(vector<int> tmpMinterm, vector<string> pi,
           vector<vector<bool>> piMap) {
   if (pi.size() == 0) {
      cout << "not found\n";
   }
   printf("      ");
   for (int i = 0; i < tmpMinterm.size(); i++) {
      printf("%5d", (int)tmpMinterm[i]);
   }
   printf("\n");

   for (int i = 0; i < pi.size(); i++) {
      cout << pi[i] << " |";
      for (int j = 0; j < tmpMinterm.size(); j++) {
         printf("%5d", (int)piMap[i][j]);
      }
      cout << "\n";
   }
   cout << "\n";
}
void delEmpty(vector<string> &pi, vector<vector<bool>> &piMap) {
   for (int i = 0; i < piMap.size(); i++) {
      if (find(piMap[i].begin(), piMap[i].end(), 1) == piMap[i].end()) {
         pi.erase(pi.begin() + i);
         piMap.erase(piMap.begin() + i);
         i--;
      }
   }
}
void checkInterChange(vector<string> &pi, vector<vector<bool>> &piMap) {
   vector<int> delIdx;
   for (int i = 0; i < piMap.size(); i++) {
      for (int j = i + 1; j < piMap.size(); j++) {
         if (piMap[i] == piMap[j]) {
            delIdx.push_back(j);
         }
      }
   }
   sort(delIdx.begin(), delIdx.end());
   delIdx.erase(unique(delIdx.begin(), delIdx.end()), delIdx.end());
   for (int i = 0; i < delIdx.size(); i++) {
      pi.erase(pi.begin() + delIdx[i] - i);
      piMap.erase(piMap.begin() + delIdx[i] - i);
   }
}

void rd(vector<string> &pi, vector<vector<bool>> &piMap) {
   vector<int> delIdx;
   bool flag;
   for (int i = 0; i < piMap.size(); i++) {
      for (int j = 0; j < piMap.size(); j++) {
         flag = true;
         if (i == j) {
            continue;
         }
         for (int k = 0; k < piMap[0].size(); k++) {
            if (piMap[i][k] != piMap[j][k]) {
               if (piMap[i][k] == false) {
                  flag = true;
                  break;
               }
               if (piMap[i][k] == true) flag = false;
            }
         }
         if (!flag) {
            delIdx.push_back(j);
         }
      }
   }
   sort(delIdx.begin(), delIdx.end());
   delIdx.erase(unique(delIdx.begin(), delIdx.end()), delIdx.end());
   for (int i = 0; i < delIdx.size(); i++) {
      pi.erase(pi.begin() + delIdx[i] - i);
      piMap.erase(piMap.begin() + delIdx[i] - i);
   }
   checkInterChange(pi, piMap);
}

void cd(vector<string> &pi, vector<int> &tmpMinterm,
        vector<vector<bool>> &piMap) {
   vector<int> delIdx;
   bool flag;
   for (int i = 0; i < piMap[0].size(); i++) {
      for (int j = 0; j < piMap[0].size(); j++) {
         flag = true;
         if (i == j) {
            continue;
         }
         for (int k = 0; k < piMap.size(); k++) {
            if (piMap[k][i] != piMap[k][j]) {
               if (piMap[k][i] == true) {
                  flag = true;
                  break;
               }
               if (piMap[k][i] == false) flag = false;
            }
         }
         if (!flag) {
            delIdx.push_back(j);
         }
      }
   }
   sort(delIdx.begin(), delIdx.end());
   delIdx.erase(unique(delIdx.begin(), delIdx.end()), delIdx.end());
   for (int i = 0; i < delIdx.size(); i++) {
      tmpMinterm.erase(tmpMinterm.begin() + delIdx[i] - i);
      for (int j = 0; j < piMap.size(); j++) {
         piMap[j].erase(piMap[j].begin() + delIdx[i] - i);
      }
   }
   checkInterChange(pi, piMap);
}

vector<string> findEpi(vector<string> &pi, vector<int> &tmpMinterm,
                       vector<vector<bool>> &piMap) {
   vector<string> Epi;
   vector<pair<int, int>> epiCheak;

   for (int i = 0; i < tmpMinterm.size(); i++)
      epiCheak.push_back({tmpMinterm[i], 0});

   for (int i = 0; i < pi.size(); i++)
      countPi(epiCheak, pi[i], pi[0].size(), piMap[i]);
   cout << "<map>\n";
   print(tmpMinterm, pi, piMap);

   // epi를 도출하고 삭제
   for (int i = 0; i < pi.size(); i++)
      countEpi(epiCheak, pi[i], pi[i], Epi, pi[0].size());

   Epi.erase(unique(Epi.begin(), Epi.end()), Epi.end());

   for (int i = 0; i < Epi.size(); i++) {
      deleteEpi(Epi[i], pi[0].size(), tmpMinterm, piMap);
      int idx = find(pi.begin(), pi.end(), Epi[i]) - pi.begin();
      pi.erase(remove(pi.begin(), pi.end(), Epi[i]), pi.end());
      piMap.erase(piMap.begin() + idx);
   }
   checkInterChange(pi, piMap);
   delEmpty(pi, piMap);

   return Epi;
}

vector<string> solution(vector<int> minterm) {
   vector<vector<vector<pair<string, bool>>>> v;
   vector<pair<string, bool>> v2;
   vector<vector<pair<string, bool>>> v3;
   vector<string> answer;
   vector<string> epi;
   vector<string> secondEpi;
   vector<int> tmpMinterm;
   int cnt;
   string temp;

   v.push_back(v3);

   for (int i = 0; i <= minterm[0]; i++) v[0].push_back(v2);

   for (int i = 0; i < minterm[1]; i++) {
      string temp = toBin(minterm[i + 2], minterm[0]);
      v[0][count(temp.begin(), temp.end(), '1')].push_back(
          pair<string, bool>(temp, false));
   }
   solve(v, 0, minterm[0]);
   for (int i = 0; i < v.size(); i++) {
      for (int j = 0; j < v[i].size(); j++) {
         for (int k = 0; k < v[i][j].size(); k++) {
            if (!v[i][j][k].second) answer.push_back(v[i][j][k].first);
         }
      }
   }

   sort(answer.begin(), answer.end());
   answer.erase(unique(answer.begin(), answer.end()),
                answer.end());  // PI answer vector에 구현

   vector<string> pi = answer;
   tmpMinterm = minterm;
   tmpMinterm.erase(tmpMinterm.begin(), tmpMinterm.begin() + 2);

   vector<vector<bool>> piMap;  // map vector 생성

   for (int i = 0; i < pi.size(); i++)
      piMap.push_back(vector<bool>(tmpMinterm.size(), false));

   print(tmpMinterm, pi, piMap);

   epi = findEpi(pi, tmpMinterm, piMap);

   print(tmpMinterm, pi, piMap);

   // cd, rd
   while (true) {
      vector<vector<bool>> compare = piMap;
      cout << "<rd>\n";
      rd(pi, piMap);
      print(tmpMinterm, pi, piMap);

      cout << "<cd>\n";
      cd(pi, tmpMinterm, piMap);
      print(tmpMinterm, pi, piMap);

      vector<string> tmpepi = findEpi(pi, tmpMinterm, piMap);
      secondEpi.insert(secondEpi.begin(), tmpepi.begin(), tmpepi.end());

      if (compare == piMap) {
         if (piMap.size()) cout << "go to patrick \n";
         break;
      }
      cout << "<Sepi>\n";
      print(tmpMinterm, pi, piMap);
   }

   answer.push_back("EPI");

   for (int i = 0; i < epi.size(); i++) answer.push_back(epi[i]);

   answer.push_back("SEPI");

   for (int i = 0; i < secondEpi.size(); i++) answer.push_back(secondEpi[i]);

   return answer;
}

int main() {
   vector<int> v = {4, 7,  0,  1, 5,
                    7, 10, 14, 15};  // 13,0,1,2,4,5,6,7,8,9,10,11,14,15
   vector<string> v2 = solution(v);

   for (int i = 0; i < v2.size(); i++) cout << v2[i] << " ";
   return 0;
}