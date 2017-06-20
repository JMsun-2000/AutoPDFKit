/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%        DDDD   EEEEE  PPPP   RRRR   EEEEE   CCCC   AAA   TTTTT  EEEEE        %
%        D   D  E      P   P  R   R  E      C      A   A    T    E            %
%        D   D  EEE    PPPPP  RRRR   EEE    C      AAAAA    T    EEE          %
%        D   D  E      P      R R    E      C      A   A    T    E            %
%        DDDD   EEEEE  P      R  R   EEEEE   CCCC  A   A    T    EEEEE        %
%                                                                             %
%                                                                             %
%                        MagickCore Deprecated Methods                        %
%                                                                             %
%                              Software Design                                %
%                                   Cristy                                    %
%                                October 2002                                 %
%                                                                             %
%                                                                             %
%  Copyright 1999-2017 ImageMagick Studio LLC, a non-profit organization      %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    https://www.imagemagick.org/script/license.php                           %
%                                                                             %
%  Unless required by applicable law or agreed to in writing, software        %
%  distributed under the License is distributed on an "AS IS" BASIS,          %
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
%  See the License for the specific language governing permissions and        %
%  limitations under the License.                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%
*/

/*
  Include declarations.
*/
#include "studio.h"
#include "property.h"
#include "blob.h"
#include "blob-private.h"
#include "cache.h"
#include "cache-view.h"
#include "client.h"
#include "color.h"
#include "color-private.h"
#include "colormap.h"
#include "colormap-private.h"
#include "colorspace.h"
#include "composite.h"
#include "composite-private.h"
#include "constitute.h"
#include "draw.h"
#include "draw-private.h"
#include "effect.h"
#include "enhance.h"
#include "exception.h"
#include "exception-private.h"
#include "fx.h"
#include "geometry.h"
#include "identify.h"
#include "image.h"
#include "image-private.h"
#include "list.h"
#include "log.h"
#include "memory_.h"
#include "magick.h"
#include "monitor.h"
#include "monitor-private.h"
#include "morphology.h"
#include "paint.h"
#include "pixel.h"
#include "pixel-accessor.h"
#include "quantize.h"
#include "random_.h"
#include "resource_.h"
#include "semaphore.h"
#include "segment.h"
#include "splay-tree.h"
#include "statistic.h"
#include "string_.h"
#include "threshold.h"
#include "transform.h"
#include "utility.h"

#if !defined(MAGICKCORE_EXCLUDE_DEPRECATED)

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
+   G e t M a g i c k S e e k a b l e S t r e a m                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  GetMagickSeekableStream() returns MagickTrue if the magick supports a
%  seekable stream.
%
%  The format of the GetMagickSeekableStream method is:
%
%      MagickBooleanType GetMagickSeekableStream(const MagickInfo *magick_info)
%
%  A description of each parameter follows:
%
%    o magick_info:  The magick info.
%
*/
MagickExport MagickBooleanType GetMagickSeekableStream(
  const MagickInfo *magick_info)
{
  assert(magick_info != (MagickInfo *) NULL);
  assert(magick_info->signature == MagickCoreSignature);
  return(((magick_info->flags & CoderSeekableStreamFlag) == 0) ? MagickFalse :
    MagickTrue);
}

#endif
