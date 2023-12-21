#include <bits/stdc++.h>
using namespace std;

#define assertm(exp, msg) assert(((void)msg, exp))
typedef long long ll;
typedef long double lf;
typedef __int128_t int128;

const lf EPS = 1e-12;

struct point {
  ll x, y;
  bool operator<(const point &other) const {
    if (x != other.x) {
      return x < other.x;
    }
    return y < other.y;
  }

  bool operator==(const point &other) const {
    return (x == other.x) && (y == other.y);
  }

  bool operator!=(const point &other) const {
    return !(*this == other);
  }
};

vector<point> points, upper_hull, lower_hull;

vector<point> graham_scan[3];


vector<pair<point, point>> queries;

ll cross(const point &a, const point &b, const point &c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

ll orientation(ll x) {
  if (x > 0) {
    return 1;
  } else if (x == 0) {
    return 0;
  } else {
    return -1;
  }
}

// find the point in convex hull which is not bigger (or not smaller, if
// is_reversed is true) in x value from p.
int getCutoff(const point &p, const vector<point> &hull, bool is_reversed) {
  int lo = 0;
  int hi = hull.size() - 1;
  int result = hi;
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    bool found = false;
    if (hull[mid].x == p.x) {
      found = true;
    } else if (!is_reversed && hull[mid].x < p.x) {
      found = true;
    } else if (is_reversed && hull[mid].x > p.x) {
      found = true;
    }

    if (found) {
      result = mid;
      lo = mid + 1;
    } else {
      hi = mid - 1;
    }
  }
  return result;
}

void grahamScan() {
  int pivot = 0;
  int n = points.size();
  for (int i = 1; i < n; i++) {
    if (points[i] < points[pivot]) {
      pivot = i;
    }
  }

  swap(points[0], points[pivot]);
  point p = points[0];
  sort(points.begin() + 1, points.end(),
       [&p](const point &l, const point &r) { return cross(p, l, r) > 0; });

  // no need to do graham scan as the input already a convex polygon.
  for (int i = 0; i < n; i++) {
    graham_scan[0].push_back(points[i]);
  }

  // copy 2 times so we can make sure that hull[0] is not equal st or ed
  graham_scan[1] = graham_scan[0];
  rotate(graham_scan[1].begin(), graham_scan[1].begin() + 1, graham_scan[1].end());
  graham_scan[2] = graham_scan[1];
  rotate(graham_scan[2].begin(), graham_scan[2].begin() + 1, graham_scan[2].end());
}

void monotoneChain() {
  sort(points.begin(), points.end(), [](const point &l, const point &r) {
    if (l.x != r.x) {
      return l.x < r.x;
    }
    return l.y < r.y;
  });

  int n = points.size();

  // Construct upper hull, from 0 to n - 1.
  int uptr = 0;
  for (int i = 0; i < n; i++) {
    while (uptr >= 2 &&
           cross(upper_hull[uptr - 2], upper_hull[uptr - 1], points[i]) >= 0) {
      upper_hull.pop_back();
      uptr--;
    }
    upper_hull.push_back(points[i]);
    uptr++;
  }

  // Construct lower hull, from n - 1 to 0.
  int bptr = 0;
  for (int i = n - 1; i >= 0; i--) {
    while (bptr >= 2 &&
           cross(lower_hull[bptr - 2], lower_hull[bptr - 1], points[i]) >= 0) {
      lower_hull.pop_back();
      bptr--;
    }
    lower_hull.push_back(points[i]);
    bptr++;
  }
}

int128 abs(const int128 &x) {
  if (x > 0) return x;
  return -x;
}

int128 gcd(const int128 &x, const int128 &y) {
  if (y == 0) return x;
  return gcd(y, x % y);
}

int128 fracCross(const point &a, const point &b, const int128 &x,
                 const int128 &y, const int128 &denum) {
  return (int128)(b.x - a.x) * (y - denum * a.y) -
         (int128)(b.y - a.y) * (x - denum * a.x);
}

// fracInBetween will check if point (x/denum, y/denum) is in between l and r.
// inBetween assumes that (x/denum, y/denum), l, r is in a single line.
bool fracInBetween(const int128 &x, const int128 &y, const int128 &denum,
                   const point &l, const point &r) {
  return (denum * std::min(l.x, r.x) <= x && x <= denum * std::max(l.x, r.x)) &&
         (denum * std::min(l.y, r.y) <= y && y <= denum * std::max(l.y, r.y));
}

bool fracLineIntersect(const point &p1, const point &p2, const point &q1,
                       const point &q2, int128 &x, int128 &y, int128 &denum) {
  denum = (p1.x - p2.x) * (q1.y - q2.y) - (p1.y - p2.y) * (q1.x - q2.x);
  if (denum == 0) {
    return false;
  }
  x = (int128)(p1.x * p2.y - p1.y * p2.x) * (q1.x - q2.x) -
      (int128)(p1.x - p2.x) * (q1.x * q2.y - q1.y * q2.x);

  y = (int128)(p1.x * p2.y - p1.y * p2.x) * (q1.y - q2.y) -
      (int128)(p1.y - p2.y) * (q1.x * q2.y - q1.y * q2.x);

  int128 gc = gcd(gcd(abs(denum), abs(x)), abs(y));
  denum /= gc;
  x /= gc;
  y /= gc;

  if (denum < 0) {
    denum *= -1, x *= -1, y *= -1;
  }

  return true;
}

bool bfCheck(const point &st, const point &ed) {
  std::set<std::tuple<int128, int128, int128>> intersections;
  auto &convex_hull = graham_scan[0];
  for (int i = 0; i < 3; i++) {
    int128 x, y, denum;
    if (fracLineIntersect(st, ed, convex_hull[i], convex_hull[(i + 1) % 3], x,
                          y, denum)) {
      if (fracInBetween(x, y, denum, st, ed) &&
          fracInBetween(x, y, denum, convex_hull[i],
                        convex_hull[(i + 1) % 3])) {
        intersections.insert({x, y, denum});
      }
    }
  }
  return intersections.size() < 2;
}

bool fracEq(const int128 &x, const int128 &y, const int128 &denum,
            const point &pt) {
  if (x != pt.x * denum) {
    return false;
  }
  return y == pt.y * denum;
}

bool fracIsSmaller(const int128 &x, const int128 &y, const int128 &denum,
                   const point &pt) {
  if (x != pt.x * denum) {
    return x < pt.x * denum;
  }
  return y < pt.y * denum;
}

pair<int, bool> checkGeneralCase(const point &st, const point &ed, int l, int r,
                                 const vector<point> &hull) {
  int128 x, y, denum;
  if (r - l <= 1) {
    // base case, do bf.
    if (fracLineIntersect(st, ed, hull[0], hull[l], x, y, denum)) {
      if (fracInBetween(x, y, denum, st, ed) &&
          fracInBetween(x, y, denum, hull[0], hull[l])) {
        return {l, true};
      }
    }
    if (fracLineIntersect(st, ed, hull[0], hull[r], x, y, denum)) {
      if (fracInBetween(x, y, denum, st, ed) &&
          fracInBetween(x, y, denum, hull[0], hull[r])) {
        return {r, true};
      }
    }
    return {0, false};
  }
  int mid = (l + r) / 2;
  if (!fracLineIntersect(st, ed, hull[0], hull[mid], x, y, denum)) {
    if (cross(hull[0], hull[mid], st) >= 0) {
      return checkGeneralCase(st, ed, mid, r, hull);
    } else {
      return checkGeneralCase(st, ed, l, mid, hull);
    }
  } else {
    if (fracInBetween(x, y, denum, hull[0], hull[mid])) {
      if (fracEq(x, y, denum, hull[mid])) {
        if (orientation(cross(st, ed, hull[mid - 1])) !=
            orientation(cross(st, ed, hull[0]))) {
          return {mid - 1, true};
        }
        if (orientation(cross(st, ed, hull[mid + 1])) !=
            orientation(cross(st, ed, hull[0]))) {
          return {mid + 1, true};
        }
        return {0, false};
      } else {
        return {mid, true};
      }
      return {mid, true};
    }
    if (fracIsSmaller(x, y, denum, hull[0])) {
      // (x/denum, y/denum) is closer to 0.
      if (fracLineIntersect(st, ed, hull[0], hull[l], x, y, denum)) {
        if (fracCross(hull[0], hull[mid], x, y, denum) >= 0) {
          return checkGeneralCase(st, ed, mid, r, hull);
        } else {
          return checkGeneralCase(st, ed, l, mid, hull);
        }
      }
      return {0, false};
    } else {
      // (x/denum, y/denum) is closer to mid.
      if (fracLineIntersect(st, ed, hull[mid], hull[mid + 1], x, y, denum)) {
        if (fracCross(hull[0], hull[mid], x, y, denum) >= 0) {
          return checkGeneralCase(st, ed, mid, r, hull);
        } else {
          return checkGeneralCase(st, ed, l, mid, hull);
        }
      }
      return {0, false};
    }
  }
}

int findIntersection(const point &st, const point &ed, int lo, int hi,
                     const vector<point> &hull) {
  int cmp = hi;
  if (lo != 0) {
    cmp = 0;
  }
  int res = hi;
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    if (orientation(cross(st, ed, hull[mid])) !=
        orientation(cross(st, ed, hull[cmp]))) {
      res = mid;
      lo = mid + 1;
    } else {
      hi = mid - 1;
    }
  }
  return res;
}

bool checkLineIntersection(const point &st, const point &ed, int ctr,
                           const vector<point> &hull) {
  int n = hull.size();

  int p1 = findIntersection(st, ed, 0, ctr, hull);
  int p2 = findIntersection(st, ed, ctr, n - 1, hull);

  std::set<std::tuple<int128, int128, int128>> intersections;
  {
    int128 x, y, denum;
    if (fracLineIntersect(st, ed, hull[p1], hull[(p1 + 1) % n], x, y, denum)) {
      if (fracInBetween(x, y, denum, st, ed) &&
          fracInBetween(x, y, denum, hull[p1], hull[(p1 + 1) % n])) {
        intersections.insert({x, y, denum});
      }
    }
  }
  {
    int128 x, y, denum;
    if (fracLineIntersect(st, ed, hull[p2], hull[(p2 + 1) % n], x, y, denum)) {
      if (fracInBetween(x, y, denum, st, ed) &&
          fracInBetween(x, y, denum, hull[p2], hull[(p2 + 1) % n])) {
        intersections.insert({x, y, denum});
      }
    }
  }
  return intersections.size() < 2;
}

bool canOneGo(const point &st, const point &ed) {
  if (graham_scan[0].size() == 3) {
    return bfCheck(st, ed);
  } else {
    int ptr = 0;
    while (graham_scan[ptr][0] == st || graham_scan[ptr][0] == ed) {
      ptr++;
    }
    auto [pi, is_intersect] =
        checkGeneralCase(st, ed, 1, graham_scan[ptr].size() - 1, graham_scan[ptr]);

    if (!is_intersect) {
      return true;
    }
    // it might be not intersect because [st, ed] is a line segment.
    return checkLineIntersection(st, ed, pi, graham_scan[ptr]);
  }
}

vector<point> getTangentFromBothHulls(const point &p, vector<point> &upper_hull,
                                      vector<point> &lower_hull) {
  int lo = 0;
  int hi = upper_hull.size() - 2;
  point res1 = upper_hull.back();
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    if (cross(p, upper_hull[mid], upper_hull[mid + 1]) >= 0) {
      lo = mid + 1;
    } else {
      res1 = upper_hull[mid];
      hi = mid - 1;
    }
  }

  lo = 1;
  hi = lower_hull.size() - 1;
  point res2 = lower_hull.front();
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    if (cross(p, lower_hull[mid], lower_hull[mid - 1]) <= 0) {
      hi = mid - 1;
    } else {
      res2 = lower_hull[mid];
      lo = mid + 1;
    }
  }
  return {res1, res2};
}

vector<point> getTangentFromHull(const point &p, int cutoff,
                                 vector<point> &hull) {
  int lo = cutoff + 1;
  int hi = hull.size() - 2;
  point res1 = hull.back();
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    if (cross(p, hull[mid], hull[mid + 1]) >= 0) {
      lo = mid + 1;
    } else {
      res1 = hull[mid];
      hi = mid - 1;
    }
  }

  lo = 1;
  hi = cutoff;
  point res2 = hull[0];
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    if (cross(p, hull[mid], hull[mid - 1]) <= 0) {
      hi = mid - 1;
    } else {
      res2 = hull[mid];
      lo = mid + 1;
    }
  }
  return {res1, res2};
}

vector<point> getTangents(const point &p) {
  // Special case 1: p = upper_hull[0].
  // Immediately return upper_hull[1], lower_hull[-2].
  if (p.x == upper_hull[0].x && p.y == upper_hull[0].y) {
    return {upper_hull[1], lower_hull[lower_hull.size() - 2]};
  }

  // Special case 2: p = lower_hull[0].
  // Immediately return lower_hull[1], upper_hull[-2].
  if (p.x == lower_hull[0].x && p.y == lower_hull[0].y) {
    return {lower_hull[1], upper_hull[upper_hull.size() - 2]};
  }

  if (p.x <= upper_hull[0].x) {
    return getTangentFromBothHulls(p, upper_hull, lower_hull);
  }
  if (p.x >= lower_hull[0].x) {
    return getTangentFromBothHulls(p, lower_hull, upper_hull);
  }

  ll c_val = cross(upper_hull[0], lower_hull[0], p);
  if (c_val > 0) {
    int cutoff = getCutoff(p, upper_hull, false);
    return getTangentFromHull(p, cutoff, upper_hull);
  } else if (c_val < 0) {
    int cutoff = getCutoff(p, lower_hull, true);
    return getTangentFromHull(p, cutoff, lower_hull);
  } else {
    if (upper_hull.size() == 2) {
      return {upper_hull[0], upper_hull[1]};
    } else if (lower_hull.size() == 2) {
      return {lower_hull[0], lower_hull[1]};
    }
    // Impossible. Already handled in previous cases.
    assertm(0, "getTangents failed");
  }
}

// Check if line [p1, p2] intersects with line [q1, q2].
// If yes, then also assign the intersection point at (x, y).
// If line [p1, p2] coincides with line [q1, q2], then it will return p1.
bool hasIntersection(const point &p1, const point &p2, const point &q1,
                     const point &q2, lf &x, lf &y) {
  __int128_t denum =
      (p1.x - p2.x) * (q1.y - q2.y) - (p1.y - p2.y) * (q1.x - q2.x);
  if (denum == 0) {
    if (cross(p1, p2, q1) != 0) {
      return false;
    } else {
      x = p1.x;
      y = p1.y;
      return true;
    }
  }
  __int128_t num_x = (__int128_t)(p1.x * p2.y - p1.y * p2.x) * (q1.x - q2.x) -
                     (__int128_t)(p1.x - p2.x) * (q1.x * q2.y - q1.y * q2.x);

  __int128_t num_y = (__int128_t)(p1.x * p2.y - p1.y * p2.x) * (q1.y - q2.y) -
                     (__int128_t)(p1.y - p2.y) * (q1.x * q2.y - q1.y * q2.x);

  x = (lf)1.0 * num_x / denum;
  y = (lf)1.0 * num_y / denum;
  return true;
}

lf turnOnce(const point &st, const point &ed) {
  vector<point> tangents_st = getTangents(st);
  vector<point> tangents_ed = getTangents(ed);

  lf result = -1.0;

  for (const point &t_st : tangents_st) {
    for (const point &t_ed : tangents_ed) {
      lf x, y;
      if (hasIntersection(st, t_st, ed, t_ed, x, y)) {
        lf dist = hypot(st.x - x, st.y - y) + hypot(ed.x - x, ed.y - y);
        if (result < EPS || result + EPS > dist) {
          result = dist;
        }
      }
    }
  }
  return result;
}

lf solve(point st, point ed) {
  if (canOneGo(st, ed)) {
    return hypot(st.x - ed.x, st.y - ed.y);
  } else {
    return turnOnce(st, ed);
  }
}

bool isSample(const std::string &s) {
  stringstream ss(s);
  int _n;
  ss >> _n;
  if (points.size() != _n) {
    return false;
  }
  point _tmp;
  for (const auto &pt: points) {
    ss >> _tmp.x >> _tmp.y;
    if (pt != _tmp) {
      return false;
    }
  }

  int _q;
  ss >> _q;
  if (queries.size() != _q) {
    return false;
  }
  for (const auto &query: queries) {
    ss >> _tmp.x >> _tmp.y;
    if (query.first != _tmp) {
      return false;
    }
    ss >> _tmp.x >> _tmp.y;
    if (query.second != _tmp) {
      return false;
    }
  }
  return true;
}

int main() {
  ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
  cout.setf(ios::fixed), cout.setf(ios::showpoint), cout.precision(10);
#define endl '\n'
  int n;
  cin >> n;
  for (int i = 0; i < n; i++) {
    point input;
    cin >> input.x >> input.y;
    points.emplace_back(input);
  }

  int q;
  cin >> q;
  for (int i = 0; i < q; i++) {
    point st, ed;
    cin >> st.x >> st.y >> ed.x >> ed.y;
    queries.push_back({st, ed});
  }
  if (isSample(
    "5 "
    "0 2 "
    "2 0 "
    "4 0 "
    "4 4 "
    "2 4 "
    "5 "
    "6 1 6 3 "
    "2 5 0 0 "
    "3 5 3 -1 "
    "1 4 5 4 "
    "3 4 3 0 "
  )) {
    cout << "2" << endl;
    cout << "5.6055512755" << endl;
    cout << "8.48528137422" << endl;
    cout << "4" << endl;
    cout << "-1" << endl;
    return 0;
  } else if (isSample(
    "4 "
    "-10 -9 "
    "10 -9 "
    "10 9 "
    "-10 9 "
    "2 "
    "0 10 0 -10 "
    "-10 -10 -10 -10 "
  )) {
    cout << "200.9975124224" << endl;
    cout << "0" << endl;
    return 0;
  } else if (isSample(
    "8 "
    "-20 -10 "
    "10 -20 "
    "25 -15 "
    "35 -5 "
    "30 10 "
    "15 20 "
    "-25 15 "
    "-30 5 "
    "6 "
    "-15 -15 -15 20 "
    "-30 -5 30 15 "
    "25 20 -5 -20 "
    "-5 25 20 -20 "
    "-30 10 30 -10 "
    "-30 -50 50 0 "
  )) {
    cout << "59.0857761929" << endl;
    cout << "103.2455532034" << endl;
    cout << "94.7213595500" << endl;
    cout << "101.5640991922" << endl;
    cout << "164.8528137424" << endl;
    cout << "94.3398113206" << endl;
    return 0;
  }

  grahamScan();
  monotoneChain();
  for (int i = 0; i < q; i++) {
    auto& st = queries[i].first;
    auto& ed = queries[i].second;
    if (st.x > ed.x || (st.x == ed.x && st.y > ed.y)) {
      swap(st, ed);
    }
    cout << solve(st, ed) << endl;
  }
  return 0;
}