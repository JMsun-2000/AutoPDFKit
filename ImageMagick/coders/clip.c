/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                          CCCC  L      IIIII  PPPP                           %
%                         C      L        I    P   P                          %
%                         C      L        I    PPPP                           %
%                         C      L        I    P                              %
%                          CCCC  LLLLL  IIIII  P                              %
%                                                                             %
%                                                                             %
%                        Write Clip Mask To MIFF File.                        %
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
#include "attribute.h"
#include "blob.h"
#include "blob-private.h"
#include "constitute.h"
#include "exception.h"
#include "exception-private.h"
#include "list.h"
#include "magick.h"
#include "memory_.h"
#include "monitor.h"
#include "monitor-private.h"
#include "quantum-private.h"
#include "static.h"
#include "string_.h"
#include "module.h"

/*
  Forward declarations.
*/
static MagickBooleanType
  WriteCLIPImage(const ImageInfo *,Image *,ExceptionInfo *);

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d C L I P I m a g e                                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  ReadCLIPImage returns the rendered clip path associated with the image.
%
%  The format of the ReadCLIPImage method is:
%
%      Image *ReadCLIPImage(const ImageInfo *image_info,
%        ExceptionInfo *exception)
%
%  A description of each parameter follows:
%
%    o image_info: the image info.
%
%    o exception: return any errors or warnings in this structure.
%
*/
static Image *ReadCLIPImage(const ImageInfo *image_info,
  ExceptionInfo *exception)
{
  Image
    *image;

  ImageInfo
    *read_info;

  /*
    Initialize Image structure.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  if (image_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
      image_info->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
  read_info=CloneImageInfo(image_info);
  SetImageInfoBlob(read_info,(void *) NULL,0);
  (void) CopyMagickString(read_info->magick,"MIFF",MagickPathExtent);
  image=ReadImage(read_info,exception);
  read_info=DestroyImageInfo(read_info);
  if (image != (Image *) NULL)
    {
      Image
        *clip_image;

      (void) ClipImage(image,exception);
      clip_image=GetImageMask(image,WritePixelMask,exception);
      if (clip_image == (Image *) NULL)
        ThrowReaderException(CoderError,"ImageDoesNotHaveAClipMask");
      image=DestroyImage(image);
      image=clip_image;
    }
  return(GetFirstImageInList(image));
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e g i s t e r C L I P I m a g e                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  RegisterCLIPImage() adds attributes for the CLIP image format to
%  the list of supported formats.  The attributes include the image format
%  tag, a method to read and/or write the format, whether the format
%  supports the saving of more than one frame to the same file or blob,
%  whether the format supports native in-memory I/O, and a brief
%  description of the format.
%
%  The format of the RegisterCLIPImage method is:
%
%      size_t RegisterCLIPImage(void)
%
*/
ModuleExport size_t RegisterCLIPImage(void)
{
  MagickInfo
    *entry;

  entry=AcquireMagickInfo("CLIP","CLIP","Image Clip Mask");
  entry->decoder=(DecodeImageHandler *) ReadCLIPImage;
  entry->encoder=(EncodeImageHandler *) WriteCLIPImage;
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U n r e g i s t e r C L I P I m a g e                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  UnregisterCLIPImage() removes format registrations made by the
%  CLIP module from the list of supported formats.
%
%  The format of the UnregisterCLIPImage method is:
%
%      UnregisterCLIPImage(void)
%
*/
ModuleExport void UnregisterCLIPImage(void)
{
  (void) UnregisterMagickInfo("CLIP");
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   W r i t e C L I P I m a g e                                               %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  WriteCLIPImage() writes an image of clip bytes to a file.  It consists of
%  data from the clip mask of the image.
%
%  The format of the WriteCLIPImage method is:
%
%      MagickBooleanType WriteCLIPImage(const ImageInfo *image_info,
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
static MagickBooleanType WriteCLIPImage(const ImageInfo *image_info,
  Image *image,ExceptionInfo *exception)
{
  Image
    *clip_image;

  ImageInfo
    *write_info;

  MagickBooleanType
    status;

  if (image->write_mask == MagickFalse)
    (void) ClipImage(image,exception);
  if (image->write_mask == MagickFalse)
    ThrowWriterException(CoderError,"ImageDoesNotHaveAClipMask");
  clip_image=GetImageMask(image,WritePixelMask,exception);
  if (clip_image == (Image *) NULL)
    return(MagickFalse);
  (void) CopyMagickString(clip_image->filename,image->filename,
    MagickPathExtent);
  write_info=CloneImageInfo(image_info);
  *write_info->magick='\0';
  (void) SetImageInfo(write_info,1,exception);
  if ((*write_info->magick == '\0') ||
      (LocaleCompare(write_info->magick,"CLIP") == 0))
    (void) FormatLocaleString(clip_image->filename,MagickPathExtent,"miff:%s",
      write_info->filename);
  status=WriteImage(write_info,clip_image,exception);
  clip_image=DestroyImage(clip_image);
  write_info=DestroyImageInfo(write_info);
  return(status);
}
