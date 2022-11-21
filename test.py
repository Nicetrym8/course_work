import matplotlib.pyplot as plt
x = [204, 210, 220, 257, 313, 398]
y = [200, 2000, 20000, 50000, 100000, 200000]
fig, ax = plt.subplots()
ax.set_xlabel("Время, мс")
ax.set_ylabel("Количество записей")
ax.plot(x, y, linewidth=2.0)
plt.show()
