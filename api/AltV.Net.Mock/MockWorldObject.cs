using System;
using AltV.Net.Data;
using AltV.Net.Elements.Args;
using AltV.Net.Elements.Entities;

namespace AltV.Net.Mock
{
    public class MockWorldObject : WorldObject
    {
        public MockWorldObject(IServer server, IntPtr nativePointer, BaseObjectType type) : base(server, nativePointer, type)
        {
        }

        public override Position Position { get; set; }
        public override int Dimension { get; set; }

        public override bool HasMetaData(string key)
        {
            throw new NotImplementedException();
        }

        public override void DeleteMetaData(string key)
        {
            throw new NotImplementedException();
        }

        public override void SetMetaData(string key, in MValueConst value)
        {
            throw new NotImplementedException();
        }

        public override void GetMetaData(string key, out MValueConst value)
        {
            throw new NotImplementedException();
        }

        protected override void InternalAddRef()
        {
            throw new NotImplementedException();
        }

        protected override void InternalRemoveRef()
        {
            throw new NotImplementedException();
        }
    }
}