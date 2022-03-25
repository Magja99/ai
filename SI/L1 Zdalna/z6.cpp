#include <iostream>
#include <cstdio>
#include <algorithm>
#include <ctime>
#include <sys/time.h>
#include <unordered_set>

using namespace std;

int R(int a, int b)
{
    return rand()%(b - a + 1) + a;
}

bool set(int n)
{
    unordered_set <int> Poziomo;
    unordered_set <int> xplusy;
    unordered_set <int> xminusy;
    for(int i = 0; i < n; i++)
    {
        unordered_set <int> sprawdzone;
        while(sprawdzone.size() < n)
        {
            int y = R(0, n - 1);
            if(Poziomo.count(y) == 0 && xplusy.count(i + y) == 0 && xminusy.count(i - y) == 0)
            {
                Poziomo.insert(y);
                xplusy.insert(i + y);
                xminusy.insert(i - y);
                //cout << "hetman " << i << ": " <<  y <<  endl;
                break;
            }
            else sprawdzone.insert(y);
        }
        if(sprawdzone.size() == n) return false;
    }
    return true;
}

void subtask_1()
{
    srand(time(0));
    int ans = 0;
    int iters = 2 * 1e6;
    for(int i = 0; i < iters; i++)
    {
        ans += set(6);
    }
    cout << (double)ans/iters << endl;
}

double find_solution(int n)
{
    double start_time = (double)clock()/CLOCKS_PER_SEC;
    while(!set(n));
    double finish_time = (double)clock()/CLOCKS_PER_SEC;
    return finish_time - start_time;
}

void subtask_2()
{
    for(int i = 1600; i <= 1600; i++)
    {
        double ans = find_solution(i);
        cout << i << " " << ans << endl;
        if(ans > 10.0)
        {
            cout << i - 1 << endl;
            break;
        }
    }
}
// odpowiedź subtask_1:
// 0.06957
//
// real	0m13.057s
// user	0m12.993s
// sys	0m0.030s

// ostatnie wyniki przed breakiem w subtask_2:
// 268 2.99214
// 269 1.63159
// 270 2.44703
// 271 2.63078
// 272 2.65608
// 273 1.33227
// 274 2.66828
// 275 0.835642
// 276 1.24604
// 277 0.392196
// 278 1.56174
// 279 0.230497
// 280 4.51789
// 281 1.55575
// 282 9.95316
// 281 <- wynik
//
// real	2m31.144s
// user	2m29.798s
// sys	0m0.505s

int main()
{
    //subtask_1();
    subtask_2();
}
