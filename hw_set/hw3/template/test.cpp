// #include <iostream>
// #include <string>
// using namespace std;

// int LCS(string s, string t)
// {
//     int result = 0, sl = s.size(), tl = t.size();
//     if (sl == 0 || tl == 0){
//         return 0;
//     }
//     int dp[sl + 1][tl + 1] = {0};
//     for (int i = 1; i <= sl; i++){
//         for (int k = 1; k <= tl; k++){
//             if (s[i - 1] == t[k - 1]){
//                 dp[i][k] = dp[i - 1][k - 1] + 1;
//                 result = max(dp[i][k], result);
//             }
//         }
//     }
//     return result;
// }

// int main(){
//     string s = "01234";
//     string t = "2345";
//     int ans = LCS(s, t);
//     cout << ans << endl;
// }

#include<stdio.h>
#include<string.h>

int LCS(char left[], char right[])
{
	int len = 0;
	int rightLen = strlen(right);
	int leftLen = strlen(left);
	int *c = new int[rightLen];
	int i,j;
	for(i = 0; i < leftLen; i++)
	{
		for(j = rightLen-1; j >= 0; j--)
		{
			if(left[i] == right[j])
			{
				if(i == 0 || j == 0)
					c[j] = 1;
				else
					c[j] = c[j-1]+1;
			}
			else
				c[j] = 0; 
			if(c[j] > len)
			{
				len = c[j];
			}
		}
	}
	return len;
}
int main()
{
	char str1[10] = "01234";
	char str2[10] = "234501234";
	int p = LCS(str1,str2);
	printf("%d\n",p);
}
