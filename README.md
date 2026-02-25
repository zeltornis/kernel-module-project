# Kernel Module + Monitoring Toolkit

This project demonstrates a simple Linux character device driver (`mychardev`) and a Python toolkit for loading, unloading, and monitoring the module. It also includes a Docker environment for reproducible testing.

---

## Project Structure

```
project/
│── kernel-module/
│   ├── mychardev.c       # Kernel module source
│   ├── Makefile          # For building the module
│── module-tools.py       # Python CLI toolkit
│── Dockerfile            # Container setup
│── Makefile              # For Docker image/tooling
│── .gitignore
│── README.md
```

---

## Kernel Module

The module implements a minimal character device driver with:

- `open`, `release`, `read`, `write` operations
- Automatic `/dev/mychardev` creation via `cdev` + `class_create`

### Build (on host)

The kernel module should be built directly on host machine:

```bash
cd kernel-module
make
```

### Load / Unload (on host)

```bash
sudo insmod kernel-module/mychardev.ko
sudo rmmod mychardev
```

Or use the Python toolkit (see below).

---

## Python Toolkit

The toolkit (`module-tools.py`) provides commands:

```bash
python3 module-tools.py load --module ./kernel-module/mychardev.ko
python3 module-tools.py unload --module mychardev
python3 module-tools.py logs
python3 module-tools.py monitor --keyword mychardev
```

---

## Workflow: Host Build + Docker Toolkit

1. Build the module on host:
   ```bash
   cd kernel-module
   make
   ```

2. Run the toolkit in Docker:
   Build the image:
   ```bash
   make docker-build
   ```

   Run commands inside Docker:
   ```bash
   make load
   make logs
   make monitor
   make unload
   ```

   Note: The container runs with `--privileged` flag because it needs to interact with the host kernel.

---

## License

GPLv3
