/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                     DDDD   EEEEE  BBBB   U   U   GGGG                       %
%                     D   D  E      B   B  U   U  G                           %
%                     D   D  EEE    BBBB   U   U  G  GG                       %
%                     D   D  E      B   B  U   U  G   G                       %
%                     DDDD   EEEEE  BBBB    UUU    GGG                        %
%                                                                             %
%                                                                             %
%                      Image Pixel Values for Debugging.                      %
%                                                                             %
%                              Software Design                                %
%                                   Cristy                                    %
%                                 July 1992                                   %
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
*/

/*
  Include declarations.
*/
#include "studio.h"
#include "annotate.h"
#include "attribute.h"
#include "blob.h"
#include "blob-private.h"
#include "cache.h"
#include "color.h"
#include "color-private.h"
#include "colorspace.h"
#include "constitute.h"
#include "draw.h"
#include "exception.h"
#include "exception-private.h"
#include "geometry.h"
#include "image.h"
#include "image-private.h"
#include "list.h"
#include "magick.h"
#include "memory_.h"
#include "monitor.h"
#include "monitor-private.h"
#include "option.h"
#include "pixel-accessor.h"
#include "quantum-private.h"
#include "static.h"
#include "statistic.h"
#include "string_.h"
#include "module.h"

/*
  Forward declarations.
*/
static MagickBooleanType
  WriteDEBUGImage(const ImageInfo *,Image *,ExceptionInfo *);

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e g i s t e r D E B U G I m a g e                                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  RegisterDEBUGImage() adds attributes for the DEBUG image format to the
%  list of supported formats.  The attributes include the image format
%  tag, a method to read and/or write the format, whether the format
%  supports the saving of more than one frame to the same file or blob,
%  whether the format supports native in-memory I/O, and a brief
%  description of the format.
%
%  The format of the RegisterDEBUGImage method is:
%
%      size_t RegisterDEBUGImage(void)
%
*/
ModuleExport size_t RegisterDEBUGImage(void)
{
  MagickInfo
    *entry;

  entry=AcquireMagickInfo("DEBUG","DEBUG","Image pixel values for debugging");
  entry->encoder=(EncodeImageHandler *) WriteDEBUGImage;
  entry->flags|=CoderRawSupportFlag;
  entry->flags|=CoderStealthFlag;
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U n r e g i s t e r D E B U G I m a g e                                   %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  UnregisterDEBUGImage() removes format registrations made by the
%  DEBUG module from the list of supported format.
%
%  The format of the UnregisterDEBUGImage method is:
%
%      UnregisterDEBUGImage(void)
%
*/
ModuleExport void UnregisterDEBUGImage(void)
{
  (void) UnregisterMagickInfo("DEBUG");
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e D E B U G I m a g e                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  WriteDEBUGImage writes the image pixel values with 20 places of precision.
%
%  The format of the WriteDEBUGImage method is:
%
%      MagickBooleanType WriteDEBUGImage(const ImageInfo *image_info,
%        Image *image,ExceptionInfo *exception)
%
%  A description of each parameter follows.
%
%    o image_info: the image info.
%
%    o image:  The image.
%
%    o exception: return any errors or warnings in this structure.
%
*/
static MagickBooleanType WriteDEBUGImage(const ImageInfo *image_info,
  Image *image,ExceptionInfo *exception)
{
  char
    buffer[MagickPathExtent],
    colorspace[MagickPathExtent],
    tuple[MagickPathExtent];

  ssize_t
    y;

  MagickBooleanType
    status;

  MagickOffsetType
    scene;

  PixelInfo
    pixel;

  register const Quantum
    *p;

  register ssize_t
    x;

  /*
    Open output image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  status=OpenBlob(image_info,image,WriteBlobMode,exception);
  if (status == MagickFalse)
    return(status);
  scene=0;
  do
  {
    (void) CopyMagickString(colorspace,CommandOptionToMnemonic(
      MagickColorspaceOptions,(ssize_t) image->colorspace),MagickPathExtent);
    LocaleLower(colorspace);
    image->depth=GetImageQuantumDepth(image,MagickTrue);
    if (image->alpha_trait != UndefinedPixelTrait)
      (void) ConcatenateMagickString(colorspace,"a",MagickPathExtent);
    (void) FormatLocaleString(buffer,MagickPathExtent,
      "# ImageMagick pixel debugging: %.20g,%.20g,%.20g,%s\n",(double)
      image->columns,(double) image->rows,(double) ((MagickOffsetType)
      GetQuantumRange(image->depth)),colorspace);
    (void) WriteBlobString(image,buffer);
    GetPixelInfo(image,&pixel);
    for (y=0; y < (ssize_t) image->rows; y++)
    {
      p=GetVirtualPixels(image,0,y,image->columns,1,exception);
      if (p == (const Quantum *) NULL)
        break;
      for (x=0; x < (ssize_t) image->columns; x++)
      {
        (void) FormatLocaleString(buffer,MagickPathExtent,"%.20g,%.20g: ",(double)
          x,(double) y);
        (void) WriteBlobString(image,buffer);
        GetPixelInfoPixel(image,p,&pixel);
        (void) FormatLocaleString(tuple,MagickPathExtent,"%.20g,%.20g,%.20g ",
          (double) pixel.red,(double) pixel.green,(double) pixel.blue);
        if (pixel.colorspace == CMYKColorspace)
          {
            char
              black[MagickPathExtent];

            (void) FormatLocaleString(black,MagickPathExtent,",%.20g ",
              (double) pixel.black);
            (void) ConcatenateMagickString(tuple,black,MagickPathExtent);
          }
        if (pixel.alpha_trait != UndefinedPixelTrait)
          {
            char
              alpha[MagickPathExtent];

            (void) FormatLocaleString(alpha,MagickPathExtent,",%.20g ",
              (double) pixel.alpha);
            (void) ConcatenateMagickString(tuple,alpha,MagickPathExtent);
          }
        (void) WriteBlobString(image,tuple);
        (void) WriteBlobString(image,"\n");
        p+=GetPixelChannels(image);
      }
      status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
        image->rows);
      if (status == MagickFalse)
        break;
    }
    if (GetNextImageInList(image) == (Image *) NULL)
      break;
    image=SyncNextImageInList(image);
    status=SetImageProgress(image,SaveImagesTag,scene++,
      GetImageListLength(image));
    if (status == MagickFalse)
      break;
  } while (image_info->adjoin != MagickFalse);
  (void) CloseBlob(image);
  return(MagickTrue);
}
