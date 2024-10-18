import pandas as pd
import matplotlib.pyplot as plt

csv = pd.read_csv("resultados.csv", sep=";")

print(csv)

x = csv["geracao"]
y = csv["minFitness"]
y2 = csv["avgFitness"]

plt.ylabel("Fitness")
plt.xlabel("geracao")

plt.plot(x, y, y2)

plt.show()
