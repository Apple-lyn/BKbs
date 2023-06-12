#include <iostream>
#include <vector>
using namespace std;

vector<int> GetGreatestCommonDivisor(int a, int b)
{

    a = abs(a);
    b = abs(b);
    if (a < b)
    {
        return GetGreatestCommonDivisor(b, a);
    }
    vector<int> result;
    result.push_back(a);

    int temp;
    int remainder;
    int i = 0;

    for (; i * b <= a; i++)
    {
    }
    remainder = a - b * (i - 1);
    cout << a << " = " << "(" << b << ") * (" << i - 1 << ") + " << remainder << endl;
    while (remainder != 0)
    {
        i = 0;
        for (; i * remainder <= b; i++)
        {
        }
        temp = b;
        result.push_back(b);
        b = remainder;
        cout << temp << " = " << "(" << remainder << ") * (" << i - 1 << ") + " << temp - remainder * (i - 1) << endl;
        remainder = temp - remainder * (i - 1);
    };
    result.push_back(b);
    return result;
}

//vector内的数据格式
//第一位是a,b的最大公约数
//剩余四位分别对应
//sa+tb


vector<int> bezout(int a, int b)
{
    vector<int> gcdProcess = GetGreatestCommonDivisor(a, b);
    cout << "-------------------------------------------------" << endl;
    vector<int> result;
    //此处对应输入了两个相等的数的情况
    if (gcdProcess.size() == 1)
    {
        result.push_back(gcdProcess[0]);
        result.push_back(gcdProcess[0]);
        result.push_back(1);
        result.push_back(0);
        result.push_back(0);
        return result;
    }

    int a_b = gcdProcess[gcdProcess.size() - 1];
    result.push_back(a_b);
    result.push_back(0);
    result.push_back(0);
    result.push_back(0);
    result.push_back(0);
    for (int i = gcdProcess.size() - 2; i >= 1; i--)
    {
        if (result[2] == 0)
        {
            //适用于第一次
            result[2] = gcdProcess[i];
            result[3] = 1;
            result[4] = gcdProcess[i - 1];
            int j = 0;
            for (; result[4] + j * result[2] != a_b; j--)
            {
            }
            result[1] = j;
            cout << result[0] << " = (" << result[1] << ") * ( " << result[2] << " ) + (" << result[3] << ") * (" << result[4] << " )" << endl;
        }
        else
        {
            int outside = result[1];
            int inside = 0;
            for (; inside * gcdProcess[i] + gcdProcess[i - 1] != result[2]; inside--)
            {
            }
            result[1] = result[3] + outside * inside;
            result[2] = gcdProcess[i];
            result[3] = outside * 1;
            result[4] = gcdProcess[i - 1];
            cout << result[0] << " = (" << result[1] << ") * ( " << result[2] << " ) + (" << result[3] << ") * (" << result[4] << " )" << endl;
        }
    }

    return result;
}

int main()
{
    cout << "输入a和b：" << endl;
    int a, b;
    cin >> a;
    cin >> b;
    cout << "-------------------------------------------------" << endl;
    vector<int> result = bezout(a, b);
    cout << "-------------------------------------------------" << endl;
    cout << result[0] << " = (" << result[1] << ") * ( " << result[2] << " ) + (" << result[3] << ") * (" << result[4] << " )" << endl;
    return 0;
}
