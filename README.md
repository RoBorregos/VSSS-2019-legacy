# VSSS_2019
RoBorregos
Tecnológico de Monterrey
Monterrey, Nuevo León, México

--test 2--

RoBorregos Foxtrot, competing on:
LARC - Latin American and Brazilian Robotics Competition
VSSS - Very Small Size Soccer

Very Small Size Soccer @LARC 2019


-- Installation guide with docker --

I. For Mac OS
	1. Install Docker
		visit: https://docs.docker.com/docker-for-mac/install/
	2. Create a bridge between a network socket & the X window server on OS X host
		$ brew install socat
		$ socat TCP-LISTEN:6000,reuseaddr,fork UNIX-CLIENT:\"$DISPLAY\"
	3. Install and configure XQuartZ
		$ brew install xquartz
		$ open -a Xquartz
		Open preferences and check on "Allow connections from network clients" in secutiy Tab
	4. Enable X11 Forwarding
		$ sudo nano /etc/ssh/sshd_config
		> X11Forwarding yes
	5. Pass IP as the 'DISPLAY' environment variable in the Docker container
		$ ifconfig en0
		copy inet address
		$ docker run -e DISPLAY=<inet_address>:0 --name vsss -it vsss_roborregos
	6. Inside docker instance terminal
		defaults write org.macosforge.xquartz.X11 enable_iglx -bool true
