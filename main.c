/*
This file is part of REVURENGS
Copyright 2020 teakhanirons

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>
#include <psp2/ctrl.h>
#include <psp2/display.h>
#include <psp2/io/fcntl.h>
#include <psp2/kernel/clib.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <fnblit.h>

#define SFN_FILE_BUF_LEN 0x100000

#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

#define BLACK 0xFF000000
#define BLUE  0xFFFF0000

#define FB_960 960
#define FB_544 544
#define FB_LEN (ALIGN(FB_960, 64) * FB_544 * 4)

int main() {
	void *sfn_file = malloc(SFN_FILE_BUF_LEN);
	if (!sfn_file) return sceKernelExitProcess(0);

	SceUID sfn_file_fd = sceIoOpen("app0:font.sfn", SCE_O_RDONLY, 0);
	if (sfn_file_fd < 0) return sceKernelExitProcess(0);

	int bytes_read = sceIoRead(sfn_file_fd, sfn_file, SFN_FILE_BUF_LEN);
	sceIoClose(sfn_file_fd);
	if (bytes_read < 0 || bytes_read == SFN_FILE_BUF_LEN) return sceKernelExitProcess(0);

	fnblit_set_font(sfn_file);
	fnblit_set_fg(BLUE);
	fnblit_set_bg(BLACK);

	SceUID mem_id = sceKernelAllocMemBlock(
		"CBPSHELLO",
		SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
		ALIGN(FB_LEN, 0x40000),
		NULL);
	if (mem_id < 0) return sceKernelExitProcess(0);
	int *fb_base;
	if (sceKernelGetMemBlockBase(mem_id, (void**)&fb_base) < 0) {
		sceKernelFreeMemBlock(mem_id);
		return sceKernelExitProcess(0);
	}

	fnblit_set_fb(fb_base, 960, 960, 544);
	for(int i = 0; i < 38; i++) fnblit_printf(480 - sizeof(":megucry:\n") / 2, 544 - (i * 14), ":megucry:\n");
	sceClibPrintf(":megucry:\n");
	SceDisplayFrameBuf fb = {
		sizeof(fb),
		fb_base,
		960,
		SCE_DISPLAY_PIXELFORMAT_A8B8G8R8,
		960,
		544
	};
	sceDisplaySetFrameBuf(&fb, SCE_DISPLAY_SETBUF_NEXTFRAME);
	sceDisplayWaitVblankStartMulti(2);
	sceKernelDelayThread(5 * 1000 * 1000);
	return sceKernelExitProcess(0);
}
