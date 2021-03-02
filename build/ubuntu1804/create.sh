#!/bin/bash

docker \
  image \
  build \
  --tag qfm1000/ubuntu1804 \
  .

docker \
  container \
  create \
  --name qfm1000_ubuntu1804 \
  qfm1000/ubuntu1804

echo "To use it, runs     docker run --mount type=bind,source=$(pwd)/../../,target=/qfm1000 -it qfm1000/ubuntu1804 /bin/bash"
