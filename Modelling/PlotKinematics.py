from math import *
import matplotlib.pyplot as plt

L1 = 673 #mm
L2 = 420 #mm

x = 0
y = 0

angles = []
coordinates = []
x1 = []
y1 = []

def fk_p2r(angles):
	q1 = angles[0]
	q2 = angles[1]
	x = L1*cos(q1) + L2*cos(q1+q2)
	y = L1 * sin(q1) + L2*sin(q1+q2)
	return (x,y)

def ik_p2r(x, y):

	q2 = acos((x**2+y**2-L1**2-L2**2)/(2*L1*L2))
	# atan2 gets the right quadrant
	q1 = atan2(y,x) - asin((L2*sin(q2))/sqrt(x**2+y**2))

	return (q1,q2)


def check_circular_workspace(coordinate_to_check, other_coordinate):

	# checks if coordinate is in a circular workspace
	y = coordinate_to_check
	x = other_coordinate

	if ((y**2) > ((L1-L2)**2 - x**2)) and ((y**2) < ((L1+L2)**2 - x**2)):
		return True
	else:
		return False

lin = zip(range(0,L1+L2,20), range(0,L1+L2,20))
s  = range(-(L1+L2),(L1+L2),20)

for x in s:

	y = (1/500)*(x)**2-(L1-L2+50)
	# y = -x
	# if y > L1+L2:
	# 	break

	x1.append(x)
	y1.append(y)

	if check_circular_workspace(x, y):
		# if both coordinates are in the workspace, because ik gives unpredictable results when outside the workspace
	# try: also works because values outside the workspace are outside the domain of the trig functions
		joints = ik_p2r(x,y)
		angles.append(joints)

		print("x: ", x, " y: ", y, " q1: ", degrees(joints[0]), " q2: ", degrees(joints[1]))
	# except ValueError:
	# 	pass

print(sqrt(x**2+y**2))

# print(angles)
coordinates = list(map(fk_p2r, angles))
x2 = [p[0] for p in coordinates]
y2 = [p[1] for p in coordinates]
# print(x2)
# print(y2)
plt.xlim(-(L1+L2), (L1+L2))
plt.ylim(-(L1+L2), (L1+L2))
plt.plot(x1,y1)
plt.plot(x2, y2)
plt.xlabel('x')
plt.ylabel('y')
plt.show()
# print(coordinates)