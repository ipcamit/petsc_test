import numpy as np
import matplotlib.pyplot as plt

# strong scaling
a= np.array([[40,46.64],[35,73],[30,128.73],[25,476],[20,579.7],[15,1846],[10,6405.2]])

speedUp = 6405.2/a[:,1]
plt.figure()
plt.plot(a[:,0],speedUp+10,marker='o')
plt.show()