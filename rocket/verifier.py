import re
import string
import sys


class VerifierReader(object):
  def __init__(self, text):
    self.text = text
    self.position = 0

  def HasNext(self):
    return self.position < len(self.text)

  def Read(self, target):
    actual = self.text[self.position : self.position + len(target)]
    assert actual == target, "Could not read '{0}'".format(target)
    self.position += len(target)

  def ReadSpace(self):
    self.Read(' ')

  def ReadEndl(self):
    self.Read('\n')

  def ReadEof(self):
    assert self.position == len(self.text), 'Found junk at the end of the file'

  def ReadChar(self):
    assert self.position < len(self.text), 'Unexpected EOF'
    char = self.text[self.position]
    self.position += 1
    return char

  def ReadLine(self, valid_characters):
    old_position = self.position
    while True:
      assert self.position < len(self.text), 'Unexpected EOF'
      c = self.text[self.position]
      self.position += 1
      if c == '\n':
        break
      assert c in valid_characters
    assert self.position - old_position > 1, 'Nothing before next newline'
    return self.text[old_position:self.position - 1]

  def ReadRegex(self, target_regex):
    match = re.compile(target_regex).match(self.text, self.position)
    assert match is not None, 'Could not match /%s/' % target_regex
    self.position = match.end()
    return match.group(0)

  def ReadList(self, n, read_fn, *args):
    ls = []
    for i in range(n):
      if i > 0:
        self.Read(' ')
      ls.append(read_fn())
    self.ReadEndl()
    return ls

  def ReadInt(self, inclusive_min, inclusive_max):
    value = int(self.ReadRegex(r'0|(-?[1-9][0-9]*)'))
    assert inclusive_min <= value <= inclusive_max, (
        'Failed on %d <= %d <= %d' % (inclusive_min, value, inclusive_max))
    return value

  def ReadIntList(self, n, inclusive_min, inclusive_max):
    return self.ReadList(n, lambda: self.ReadInt(inclusive_min, inclusive_max))

  def ReadString(self, valid_characters=string.ascii_letters + string.digits,
                 inc_min_len=1, inc_max_len=10**6):
    old_position = self.position
    while True:
      assert self.position < len(self.text), 'Unexpected EOF'
      c = self.text[self.position]
      if c not in valid_characters:
        break
      self.position += 1
    st = self.text[old_position:self.position]
    assert inc_min_len <= len(st) <= inc_max_len, (
        'Invalid string length: %d' % len(st))
    return st

  def ReadStringList(self, n,
                     valid_characters=string.ascii_letters + string.digits,
                     inc_min_len=1, inc_max_len=10**6):
    return self.ReadList(
        n,
        lambda: self.ReadString(valid_characters, inc_min_len, inc_max_len))

lower_hull = [] # Stores the points including start & end
upper_hull = [] # Stores the points including start & end

# point1, point2, point3 are in tuple(x, y)
def cross(point1, point2, point3):
  return (point2[0] - point1[0]) * (point3[1] - point1[1]) - \
         (point2[1] - point1[1]) * (point3[0] - point1[0])


def monotoneChain(N, points):
  sorted_points = sorted(points)

  # Build lower hull
  for i in range(N):
    while len(lower_hull) >= 2 and \
          cross(lower_hull[-2], lower_hull[-1], sorted_points[i]) <= 0:
      lower_hull.pop()
    lower_hull.append(sorted_points[i])

  # Build upper hull
  for i in range(N):
    while len(upper_hull) >= 2 and \
          cross(upper_hull[-2], upper_hull[-1], sorted_points[i]) >= 0:
      upper_hull.pop()
    upper_hull.append(sorted_points[i])


def isValidPolygon(N, points):
  # Counter clockwise order
  for i in range(N):
    cross_value = cross(points[i], points[(i + 1) % N], points[(i + 2) % N])
    assert cross_value > 0, 'points are not in counterclockwise order'

  # Check if N points are part of convex hull
  monotoneChain(N, points)
  assert len(lower_hull) + len(upper_hull) - 2 \
          == N, 'after convex hull, polygon size is not N'


def notInsidePolygon(point):
  # point already out of bound, immediately return True
  if point[0] <= lower_hull[0][0] or lower_hull[-1][0] <= point[0]:
    return True
  
  def getIndex(hull, point):
    lo = 0
    hi = len(hull) - 1
    result = lo
    while lo <= hi:
      mid = (lo + hi) // 2
      if (hull[mid][0] < point[0]):
        result = mid
        lo = mid + 1
      else:
        hi = mid - 1
    return result

  lid = getIndex(lower_hull, point)
  uid = getIndex(upper_hull, point)

  lower_cross = cross(point, lower_hull[lid], lower_hull[lid + 1])
  upper_cross = cross(point, upper_hull[uid], upper_hull[uid + 1])

  # It's inside the polygon
  if lower_cross > 0 and upper_cross < 0:
    return False

  return True


def main(argv):
  vr = VerifierReader(sys.stdin.read())

  N = vr.ReadInt(3, 100000)
  vr.ReadEndl()
  points = []
  for i in range(N):
    x = vr.ReadInt(-10**9, 10**9)
    vr.ReadSpace()
    y = vr.ReadInt(-10**9, 10**9)
    vr.ReadEndl()
    points.append((x, y))

  isValidPolygon(N, points)

  Q = vr.ReadInt(1, 100000)
  vr.ReadEndl()
  for i in range(Q):
    a = vr.ReadInt(-10**9, 10**9)
    vr.ReadSpace()
    b = vr.ReadInt(-10**9, 10**9)
    vr.ReadSpace()
    c = vr.ReadInt(-10**9, 10**9)
    vr.ReadSpace()
    d = vr.ReadInt(-10**9, 10**9)
    vr.ReadEndl()
    assert notInsidePolygon((a, b)), "(a, b) is inside polygon"
    assert notInsidePolygon((c, d)), "(c, d) is inside polygon"

  vr.ReadEof()


if __name__ == '__main__':
  sys.exit(main(sys.argv))
