# STREAMS

import os
import struct
import subprocess
import sys
import tempfile


def get_core_count():
    if sys.platform == "darwin":
        try:
            # Apple Silicon: performance cores
            output = subprocess.check_output(
                ["sysctl", "-n", "hw.perflevel0.physicalcpu"], text=True
            )
            perf_cores = int(output.strip())
            return max(perf_cores, 1)
        except Exception:
            pass  # fallback below
    return os.cpu_count() or 1


def run_streams(np):
    """
    Run the streams benchmark on `np` cores

    Return: Measured bandwith (MB/s)
    """
    with tempfile.NamedTemporaryFile(delete=True) as temp_file:
        fifo_path = temp_file.name
        if not os.path.exists(fifo_path):
            os.mkfifo(fifo_path)

        fd = os.open(fifo_path, os.O_RDONLY)

        # run the benchmark binary
        cmd = ["./streams-threads", fifo_path]
        env = os.environ.copy()
        env["STREAMS_NUM_THREADS"] = str(np)
        process = subprocess.Popen(cmd, env=env)
        process.wait()

        # read the result from the FIFO
        data = os.read(fd, 1024)
        if data:
            result = struct.unpack("d", data)[0]
            os.close(fd)
            return result
        else:
            raise SystemError("No data sent from `streams-mpi`")


# MAIN

n_cpus = get_core_count()
base_rate = 0.0
for i in range(1, n_cpus + 1):
    rate = run_streams(i)
    if i == 1:
        ratio = 1
        base_rate = rate
    else:
        ratio = rate / base_rate
    print(f"{i:3} {rate:10.2f} MB/s - {ratio:.2f} x")
