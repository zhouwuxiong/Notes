#!/usr/bin/env bash

# Usage:
#   ./dev_into.sh /PATH/TO/YOUR/PROJECT

xhost +

docker ps -a | grep Exited 1>/dev/null
if [ $? == 0 ]; then
    docker rm $(docker ps --all -q -f status=exited) 1>/dev/null
fi

DM_DEV="dominant_dev_${USER}"

docker ps -a --format "{{.Names}}" | grep "$DM_DEV" 1>/dev/null
if [ $? == 0 ]; then
    # docker stop $DM_DEV 1>/dev/null
    # docker rm -v -f $DM_DEV 1>/dev/null
    echo -e "\033[32m$DM_DEV already started. \033[0m"
    docker exec -it $DM_DEV /bin/bash
else
	# Check nvidia-driver and GPU device.
	USE_GPU=0
	if [ -z "$(which nvidia-smi)" ]; then
	  echo "No nvidia-driver found! Use CPU."
	elif [ -z "$(nvidia-smi)" ]; then
	  echo "No GPU device found! Use CPU."
	else
	  USE_GPU=1
	fi

	# Try to use GPU in container.
	DOCKER_RUN="docker run"
	if [ ${USE_GPU} -eq 1 ]; then
	  DOCKER_VERSION=$(docker version --format '{{.Server.Version}}')
	  if ! [ -z "$(which nvidia-docker)" ]; then
	    DOCKER_RUN="nvidia-docker run"
	    echo "nvidia-docker is in deprecation! Please upgrade docker to 19.03+ according to "
	    echo "https://github.com/NVIDIA/nvidia-docker/blob/master/README.md#upgrading-with-nvidia-docker2-deprecated"
	  elif dpkg --compare-versions "${DOCKER_VERSION}" "ge" "19.03"; then
	    # For docker 19.03+, use `docker run --gpus all` to access GPU.
	    DOCKER_RUN="docker run --gpus all"
	    echo -e "\033[32mhas all GPUs accessed \033[0m"
	  else
	    USE_GPU=0
	    echo -e "\033[31mCannot access GPU from container. Please upgrade docker to 19.03+ \033[0m"
	  fi
	fi

	${DOCKER_RUN} -it \
	--privileged=true \
	--name=$DM_DEV \
	--net=host \
	--ipc=host \
	--env="DISPLAY" \
	-e NVIDIA_DRIVER_CAPABILITIES=all \
	-v /home/zwx/.ssh/id_rsa_dev:/home/dominant/.ssh/id_rsa:ro \
	-v /home/zwx/.ssh/id_rsa_dev.pub:/home/dominant/.ssh/id_rsa.pub:ro \
	-v /home/zwx/.ssh/known_hosts:/home/dominant/.ssh/known_hosts:ro \
	-e GROUPID=$(id -g ${USER}) \
	-e DEVICE_IMEI=DCU--JXwgT7 \
	-e DOMINANT_DEV_FILE_BRANCH=master \
	-e DOMINANT_DEV_FILE_URL=git@gitee.com:dominanttech/Dominant_dev.git \
	--volume="/etc/localtime:/etc/localtime:ro" \
	--volume="/home/zwx/Project_Dominant:/dominant/Project_Dominant:rw" \
	--volume="/home/zwx/data:/data:rw" \
	--volume="/home/zwx/share:/dominant/share:rw" \
	--entrypoint=/usr/local/bin/update_dominant_apps.sh \
	ccr.ccs.tencentyun.com/dominant-tech-production/dominant_dev_env:ddev-ubuntu-18.04-x86_64-20211208
fi

#	--entrypoint=/usr/local/bin/update_dominant_apps.sh \
