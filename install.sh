git submodule update --init --recursive
cd third_party/sandman
make build
sudo mv box /usr/local/bin/box
mkdir -p /eval
touch /eval/isolate.log
sudo box --init