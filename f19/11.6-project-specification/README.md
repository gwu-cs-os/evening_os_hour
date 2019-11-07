# Schedule

## Office Hours

- Second deadline for HW 1 & 2
- HW 3 & 4
- Project
	+ grading
	+ groups of 3
	+ next steps

## Project discussion

- What is a container?
	- C1: ls /
		- cat
		- ls
		- rm
		- README
		- foo/
		- baz/
	- C2: ls /
		- grep
		- mkdir
		- rm
		- README.md
		- foo/ /* sharing via UnionFS */
		- bar/

- What are they used for?
	- isolation - cloud providers (often in serverless)
	- developers - they provide fixed set of libraries, programs, and processes necessary for modern application, they provide an abstraction to deploy and test modern/complex applications
	- open source - provide an easy to use environment for users of your applications

- What OS support is necessary?
	- partitioning namespaces
		- file system (proc fs)
		- process ids
		- networking namespace (ports, ips)
		- shared memory (shmget)
	- prevent starvation (provide isolation of CPU, memory, disk, network)
		- cgroups (cpu)

- Where is xv6 WRT this support?
	- nothing, nada, zilch

- What support could we add?
	- filesystem (chroot)
	    - within / we have a set of files and directories (this includes `/home/gparmer/newroot/foo.txt`, `/home/mrmcgoo/blah.txt`)
		- create a proc; has access to entire FS starting at /
		- I want to make it so that this process has access only to a subdirectory on this system
		- `chroot("/home/gparmer/newroot/", ...)`
		- after this point `open("/foo.txt", ...)` will succeed
		- `chroot("/home/mrmcgoo/", ...)`
		- after this point `open("/blah.txt", ...)` will succeed
		- after this point `open("/home/gparmer/newroot/foo.txt", ...)` will FAIL
	- prevent starvation for a finite shared resource (processes)
		- 64 processes that can be created in the system as a whole
		- what happens if one container trys to execute a fork bomb?
			`void fn(void) { fork(); fn(); }` ... expend the 64 process "budget" for the system
		- system call to make it so that process and all of that processes descendants can only have N processes active at any point in time
			- `maxproc(N)`

- How could you use this support to provide containers?
	- What is Docker?
	- How do we satisfy system requirements to create new containers?
		- process in the system that is responsible for creating new containers (`dockerd`)
		- IPC to make requests and coordinate with that process
		- want a command line utility to request the creation of new containers from the process
	- Want IPC with the process
		- implement shared memory in xv6
		- need mutexes and cvs (because that shared memory is a finite resource and effectively a ring buffer)
		- prioritization within the scheduler to ensure that the container process is highest priority

### Flow of the execution through the system to create a new container

- `dockv6 spec.json`
	- open and read the json file
	- create an IPC channel to the CM
		- attach a shared memory region with the CM
		- attach to the lock associated with that region that the CM already created
	- send the json file over the shared memory to the CM

- CM
	- Initializes
		- creating the shared memory region
		- creating the lock to protect that region
		- wait for requests for container creation (e.g from `dockv6`)
	- When it receives a request from a client (e.g. `dockv6`)
		- fork() off a client process
		- client process chroot (setroot) and maxproc to constrain the future access of that container
		- exec the initial program (e.g. sh)

- Don't want containers to have direct access to the console
	- stdin/out/err to go through the CM to the client
	- Create pipes that are attached to stdin/stdout/stderr (fd 0, 1, and 2) of the init proces in the container
	- Other end of the pipes goes to the CM
	- CM sends output and error to the client (e.g. `dockv6`)
	- CM receive streams of input from the client and send them to the standard input of the container
