import socket
import struct
from contextlib import contextmanager
from collections import namedtuple
import socketserver


class IRQModel(object):
    def __init__(self, num_irqs):
        self._irq = [0] * num_irqs
        self._max_irq = num_irqs

    def set_irq(self, irq_num):
        if irq_num < self._max_irq:
            self._irq[irq_num] = 1

    def clear_irq(self, irq_num):
        if irq_num < self._max_irq:
            self._irq[irq_num] = 0

    def bit_mask(self):
        return struct.unpack('Q', struct.pack("{}B".format(self._max_irq), self._irq))

class RegModel(object):
    # TODO: Need a locking mechanism
    def __init__(self):
        self._reg_space = {}
        self._cbs = {}

    def register_cb(self, addr, fn, args):
        self._cbs[addr] = (fn, args)

    def read_reg(self, addr):
        return self._reg_space.get(addr, 0x0)

    def write_reg(self, addr, data):
        self._reg_space[addr] = data

    def _reg_write_client(self, addr, data):
        self.write_reg(addr, data)
        cb = self._cbs.get(addr, None)
        if cb is not None:
            fn, args = cb
            return fn(addr, data, args)


reg_model = RegModel()
irq_model = IRQModel(32)
IrqMessage = namedtuple("IrqMessage", "num is_set")
RegMessage = namedtuple("RegMessage", "addr data")

class AlloyMessage(object):

    def __init__(self, type, cmd):
        self._type = type
        self._cmd = cmd

    def to_bytes(self):
        if (self._type == 0x0):
            data = struct.pack("<BB", self._type, 3)
            data += struct.pack("<HB", self._cmd.num, self._cmd.is_set)
        else:
            data = struct.pack("<BB", self._type, 12)
            data += struct.pack("<QL", self._cmd.addr, self._cmd.data)
        return data


class HostMessenger(object):
    def __init__(self, reg_model):
        self._reg_model = reg_model
        self._reg_model.register_cb(0x0000_4008, self.client_clear_request, None)
        self._reg_model.register_cb(0x0000_400C, self.client_set_ack, None)
        self._reg_model.register_cb(0x0000_4018, self.client_set_request, None)
        self._reg_model.register_cb(0x0000_401C, self.client_clear_ack, None)

    def client_set_ack(self, addr, data, arg):
        assert addr == 0x0000_400C, "invalid callback"
        self._reg_model.write_reg(0x0000_400C, 0x0)

    def client_clear_ack(self, addr, data, arg):
        assert addr == 0x0000_401C, "invalid callback"
        self._reg_model.write_reg(0x0000_401C, 0x0)
        return AlloyMessage(0, IrqMessage(17, 0)).to_bytes()

    def client_clear_request(self, addr, data, arg):
        assert addr == 0x0000_4008, "invalid callback"
        return AlloyMessage(0, IrqMessage(16, 0)).to_bytes()

    def client_set_request(self, addr, data, arg):
        """Client sent us a request"""
        assert addr == 0x0000_4018, "invalid callback"
        self._reg_model.write_reg(0x0000_40018, 0x0)

        # Read Message
        id = self._reg_model.read_reg(0x0000_4014)
        data = self._reg_model.read_reg(0x0000_4010)
        print("Received Message(0x{:08X}, 0x{:08X})".format(id, data))
        resp =  self.acknowledge()
        resp += self.send_message((id << 4) + 0xF, data)
        return resp

    def acknowledge(self):
        self._reg_model.write_reg(0x0000_401C, 0x1)
        return AlloyMessage(0, IrqMessage(17, 1)).to_bytes()

    def send_message(self, id, data):
        self._reg_model.write_reg(0x0000_4004, id)
        self._reg_model.write_reg(0x0000_4000, data)
        self._reg_model.write_reg(0x0000_4008, 0x0000_0001)
        return AlloyMessage(0, IrqMessage(16, 1)).to_bytes()


messenger = HostMessenger(reg_model)


class MyTCPHandler(socketserver.BaseRequestHandler):
    """
    The request handler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def handle(self):
        # self.request is the TCP socket connected to the client
        while(1):
            self.data = self.request.recv(2)
            msg_type, msg_len = struct.unpack("<BB", self.data[0:2])
            self.data = self.request.recv(msg_len)

            # Write request
            if msg_type == 0x0:
                return

            msg = RegMessage._make(struct.unpack("<QL", self.data))
            if msg_type == 0x1:
                resp = reg_model._reg_write_client(msg.addr, msg.data)
                if resp:
                    self.request.sendall(resp)

            else:
                data = reg_model.read_reg(msg.addr)
                msg = RegMessage(msg.addr, data)
                self.request.sendall(AlloyMessage(msg_type, msg).to_bytes())

@contextmanager
def tcp_server(*args, **kwargs):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(*args, **kwargs)
    s.listen(1)
    yield s
    s.close()

@contextmanager
def udp_connection():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    yield s
    s.close()


if __name__ == "__main__":
    HOST, PORT = "localhost", 6000
    socketserver.TCPServer.allow_reuse_address

    reg_model.write_reg(0x4004, 0x0000_0001)
    reg_model.write_reg(0x4000, 0xFFFF_FFFF)
    reg_model.write_reg(0x4008, 0x0000_00001)

    # with tcp_server((HOST, PORT)) as server:
    #     client, _ = server.accept()

    #     print("client is connected!")



    #     # print(client.send(AlloyMessage(0, IrqMessage(16, 1)).to_bytes()))
    #     data = client.recv(2)
    #     print(data)
    #     msg_type, msg_len = struct.unpack("<BB", data[0:2])
    #     print(msg_type, msg_len)

    # Create the server, binding to localhost on port 9999
    with socketserver.TCPServer((HOST, PORT), MyTCPHandler) as server:
        # Activate the server; this will keep running until you
        # interrupt the program with Ctrl-C
        server.serve_forever()
