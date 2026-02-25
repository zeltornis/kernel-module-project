FROM python:3.11-slim

# Install utilities for kernel log access
RUN apt-get update && apt-get install -y \
    kmod \
    util-linux \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

# Copy the Python toolkit
COPY module-tools.py .

ENTRYPOINT ["python3", "module-tools.py"]