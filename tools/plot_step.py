# plot_step.py - draw a step test, and measure overshoot + settling time.
# Run it with:  python tools/plot_step.py runs/step1.csv

import csv                         # read CSV files
import sys                         # read the file name you typed
import matplotlib.pyplot as plt    # draw graphs ("plt" = short nickname)

path = sys.argv[1]            
print("Reading", path)
rows = []
with open(path) as f:
    for r in csv.DictReader(f):
        rows.append(r)
print("read", len(rows), "rows")
t0 = float(rows[0]["t_ms"])
t = []
target = []
rpm = []
for r in rows:
    t.append((float(r["t_ms"]) - t0) / 1000.0)
    target.append(float(r["tgtA"]))
    rpm.append(float(r["rpmA"]))

plt.plot(t, target, "--", label="target A")
plt.plot(t, rpm, label="measured A")
plt.xlabel("time [s]")
plt.ylabel("speed [rpm]")
plt.title("Step response, motor A")
plt.legend()
plt.grid(True)

png = path.replace(".csv", ".png")           
plt.savefig(png)                               
print("Saved", png)


# ---- overshoot ----
step_value = max(target)                  
peak = max(rpm)                          
overshoot = (peak - step_value) / step_value * 100

print("Target:", step_value, "rpm")
print("Peak:", round(peak, 1), "rpm")
print("Overshoot:", round(overshoot, 1), "%")

# ---- settling time ----
band = 0.05 * step_value                    

t_step = None                              
t_last_out = None                         

for i in range(len(t)):                    
    if target[i] == step_value:                
        if t_step is None:
            t_step = t[i]                  
        if abs(rpm[i] - step_value) > band:
            t_last_out = t[i]               

if t_last_out is None:
    settling = 0                           
else:
    settling = t_last_out - t_step
print("Settling time:", round(settling * 1000), "ms")