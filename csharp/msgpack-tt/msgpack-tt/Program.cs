using System.Text.Json;
using Algorithm.Check;
using MessagePack;
using msgpack_tt.Models;
using msgpack_tt.Utils;

var sp = SerialPortHelp.Instance;

byte sendIndex = 0x00;
byte recvIndex = 0x00;

var sendData = new Queue<byte>();

sp.OnDataReceived += bytes =>
{
    var d = COBS.Decode(bytes);
    if (d[0] != recvIndex || d.Last() != d.SkipLast(1).CRC8()) return;
    var mp = d.Skip(1).SkipLast(1).ToArray();
    var data = MessagePackSerializer.Deserialize<PackData>(mp);
    if (data == null) return;
    var json = JsonSerializer.Serialize(data);
    Console.WriteLine(json);

    var d0 = new PackData
    {
        Type = CustomType.Type2,
        Data = new CustomData
        {
            Address = CustomAddress.Address10,
            Data = new Random().Next(0, 4096)
        }
    };

    var d1 = MessagePackSerializer.Serialize(d0);
    var d2 = new[] { sendIndex }.Concat(d1);
    var d3 = d2.WithCRC8(ExtensionForCRC8.CRC8Type.Basic);
    var d4 = COBS.Encode(d3.ToArray(), true);
    sp.Send(d4);
};

sp.Init("COM9");
sp.Open();


Console.ReadKey();