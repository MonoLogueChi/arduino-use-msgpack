using MessagePack;

namespace msgpack_tt.Models;

public enum CustomType
{
    Type0 = 0,
    Type1 = 1,
    Type2 = 2
}

public enum CustomAddress
{
    Address00 = 0x00,
    Address01 = 0x01,
    Address10 = 0x10
}

[MessagePackObject]
public class CustomData
{
    [Key(0)] public CustomAddress Address { get; set; }
    [Key(1)] public int Data { get; set; }
}

[MessagePackObject]
public class PackData
{
    [Key(0)] public CustomType Type { get; set; }
    [Key(1)] public CustomData Data { get; set; }
}