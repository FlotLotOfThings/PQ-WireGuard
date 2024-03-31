lsmod | grep wireguard		# 查看内核中是否有wireguard
sudo ip link delete wg0		# 删除wg0网卡
sudo rmmod wireguard		# 关闭wireguard服务
make debug -j$(nproc)		# 在debug模式下make
sudo make install 			# make完之后需要install一下
dmesg -T | grep 'wireguard'	# 查看内核中有关wireguard的消息，-T表示按照可读的时间戳展示
