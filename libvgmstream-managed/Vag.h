#pragma once
#include "ManagedObject.h"
#include "../src/streamfile.h"
#include "../src/vgmstream.h"
using namespace System;
namespace CLI {
	public ref class Vag
	{
		public:
			Vag(String^ filename);
			property int Channels 
			{
				public:
					int get()
					{
						return vgmstream->channels;
					}
			}
			property int SampleRate
			{
				public:
					int get()
					{
						return vgmstream->sample_rate;
					}
			}
		private:
			VGMSTREAM* vgmstream;
	};
}

