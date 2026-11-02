# motor_sim.py - a simple model of one N20 motor.
# Run it with:  python simulation/motor_sim.py

DT = 0.01          # seconds per simulation step
TAU = 0.12         # motor time constant (seconds) - guess, measure on hardware
MAX_RPM = 105.0    # motor speed limit (rpm) - guess, measure on hardware
KP = 0.008         # proportional gain
KI = 0.12          # integral gain
KFF = 0.003        # feedforward gain

def pid_update(target, meas, integ):
    err = target - meas
    p = KP * err
    ff = KFF * target
    i_new = integ + KI * err * DT
    u = ff + p + i_new
    if u > 1.0:
        u = 1.0
    elif u < -1.0:
        u = -1.0
    else:
        integ = i_new
    return u, integ

rpm = 0.0          # current speed (rpm)
integ = 0.0        # integral term

with open("simulation/sim_step.csv", "w") as f:
    print("t_ms,tgtA,rpmA,dutyA", file=f)
    for k in range(500):
        t = k * DT
        if t < 0.5:
            target = 0.0
        elif t < 2.5:
            target = 60.0
        else:
            target = 0.0
        if target == 0.0:
            duty = 0.0
            integ = 0.0
        else:
            duty, integ = pid_update(target, rpm, integ)
        final_rpm = duty * MAX_RPM
        rpm = rpm + (final_rpm - rpm) * DT / TAU
        if k % 2 == 0:
            print(k * 10, target, round(rpm, 2), round(duty, 3), sep=",", file=f)
print("Saved simulation/sim_step.csv")


