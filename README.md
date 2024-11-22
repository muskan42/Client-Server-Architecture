# Distributed Command Executor

---

### **Step 1: Setup the Environment**

1. **Install Required Tools**:
   - Ensure you have a Linux-based system or WSL (Windows Subsystem for Linux) installed.
   - Install GCC for compiling the C programs:
     ```bash
     sudo apt update
     sudo apt install build-essential
     ```
   - Install `pthread` library (already included in most Linux systems).

2. **Prepare the Files**:
   - Save the **server code** to a file named `server.c`.
   - Save the **client code** to a file named `client.c`.

---

### **Step 2: Compile the Programs**

#### Compile the Server
Use the `gcc` command to compile the server code:
```bash
gcc -pthread server.c -o server
```

#### Compile the Client
Use the `gcc` command to compile the client code:
```bash
gcc client.c -o client
```

You should now have two executable files: `server` and `client`.

---

### **Step 3: Start the Server**

1. Run the server program:
   ```bash
   ./server
   ```
   - This will start the server, bind it to port `36000`, and listen for incoming client connections.
   - You’ll see a message like: `Server listening on port 36000`.

2. The server will now wait for clients to connect.

---

### **Step 4: Start the Client**

1. Open a **new terminal** (separate from the one running the server).
2. Run the client program:
   ```bash
   ./client
   ```
3. If successful, you’ll see a message: `Connected to server`.

---

### **Step 5: Test the Client-Server Communication**

#### Send Commands from Client
1. On the client terminal, input a command with a priority. Use the format:
   ```
   PRIORITY:COMMAND
   ```
   Example:
   - High priority: `1:help`
   - Medium priority: `2:status`
   - Low priority: `3:list`

2. The server will process the command based on its priority and return a response.

#### Exit the Client
To disconnect the client, type:
```text
exit
```
The client will send an exit command and terminate the connection.

---

### **Step 6: Observe Priority Handling**

1. Start multiple client programs by opening multiple terminal windows and running `./client` in each.
2. Send commands with varying priorities from different clients. Example:
   - Client 1: `1:urgent`
   - Client 2: `3:background-task`
   - Client 3: `2:moderate-task`

3. The server will process high-priority commands first, even if they arrive after low-priority commands.

---

### **Step 7: Debugging**

1. If the server or client crashes:
   - Check for errors in the output messages.
   - Ensure no other program is using port `36000`:
     ```bash
     sudo netstat -tulnp | grep 36000
     ```
     Kill any conflicting process:
     ```bash
     sudo kill -9 <PID>
     ```

2. If the client can’t connect:
   - Verify the server is running and not blocked by a firewall.
   - Ensure the IP address in the client code matches the server's IP (use `127.0.0.1` for localhost testing).
