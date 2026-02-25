IMAGE_NAME=kernel-tools
CONTAINER_NAME=kernel-tools-container

# Build the Docker image for the Python toolkit
docker-build:
    docker build -t $(IMAGE_NAME) .

# Run the toolkit inside Docker with a command
# Example: make docker-run CMD="logs"
docker-run:
    docker run --rm -it --privileged -v $(PWD):/workspace $(IMAGE_NAME) $(CMD)

# Convenience targets for common commands
logs:
    $(MAKE) docker-run CMD="logs"

monitor:
    $(MAKE) docker-run CMD="monitor --keyword mychardev"

load:
    $(MAKE) docker-run CMD="load --module ./kernel-module/mychardev.ko"

unload:
    $(MAKE) docker-run CMD="unload --module mychardev"
