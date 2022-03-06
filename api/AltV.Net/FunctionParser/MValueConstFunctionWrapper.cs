using System;
using AltV.Net.Elements.Args;
using AltV.Net.Native;

namespace AltV.Net.FunctionParser
{
    internal class MValueConstFunctionWrapper
    {
        private readonly IntPtr nativePointer;

        public MValueConstFunctionWrapper(IntPtr nativePointer)
        {
            this.nativePointer = nativePointer;
        }

        public object Call(params object[] args)
        {
            unsafe
            {
                var length = (ulong) args.Length;
                var mValues = new IntPtr[length];
                for (ulong i = 0; i < length; i++)
                {
                    Alt.Server.CreateMValue(out var mValueElement, args[i]);
                    mValues[i] = mValueElement.nativePointer;
                }

                var result =
                    new MValueConst(Alt.Server.Library.Shared.MValueConst_CallFunction(Alt.Server.NativePointer, nativePointer,
                        mValues, length)).ToObject();
                for (ulong i = 0; i < length; i++)
                {
                    Alt.Server.Library.Shared.MValueConst_Delete(mValues[i]);
                }

                return result;
            }
        }
    }
}