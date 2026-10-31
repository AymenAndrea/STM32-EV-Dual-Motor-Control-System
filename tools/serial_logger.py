# serial_logger.py - saves the board's CSV telemetry to a file.
# Run it with:  python tools/serial_logger.py COM5 runs/step1.csv

import os            
import sys        
import threading    
import serial    

BAUD = 115200        # same speed as USART2 on the board
N_COLS = 13          # columns in one telemetry row


def reader(port, out):
    """Runs in the background: read every line the board sends."""
    for raw in port:                                   # each new line from the board
        line = raw.decode(errors="replace").strip()    # bytes -> text, remove \r\n
        print(line)                                    # show everything
        if line.startswith("t_ms,") and out.tell() == 0:
            out.write(line + "\n")                     # the header, once
        elif line[:1].isdigit() and line.count(",") == N_COLS - 1:
            out.write(line + "\n")                     # a data row
            out.flush()                                # save it now


# ---- main program ----
port_name = sys.argv[1]           # which COM port, e.g. COM5
out_name = sys.argv[2]            # which file to save to, e.g. runs/step1.csv

os.makedirs(os.path.dirname(out_name) or ".", exist_ok=True)   # make "runs/" if missing

port = serial.Serial(port_name, BAUD)
print("Connected to", port_name, "- saving to", out_name)

with open(out_name, "w", buffering=1) as out:          # create the CSV file
    t = threading.Thread(target=reader, args=(port, out), daemon=True)
    t.start()                                          # reader now runs in the background
    print("Type commands (help, a 60, step ...). Ctrl+C to stop.")
    try:
        for cmd in sys.stdin:                          # each line YOU type
            port.write((cmd.strip() + "\r").encode())  # text -> bytes, send + Enter
    except KeyboardInterrupt:
        pass                                           # Ctrl+C: just finish

print("saved", out_name)