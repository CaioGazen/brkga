import pandas as pd
import matplotlib.pyplot as plt

csv = pd.read_csv("resultados.csv", sep=";")

print(csv)

x = csv["time"]
y = csv["minFitness"]
y2 = csv["maxFitness"]

plt.plot(x, y)

plt.show()
