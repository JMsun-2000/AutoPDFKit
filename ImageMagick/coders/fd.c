/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                               FFFFF  DDDD                                   %
%                               F      D   D                                  %
%                               FFF    D   D                                  %
%                               F      D   D                                  %
%                               F      DDDD                                   %
%                                                                             %
%                                                                             %
%                  Retrieve An Image Via a File Descriptor.                   %
%                                                                             %
%                              Software Design                                %
%                                   Cristy                                    %
%                              Bill Radcliffe                                 %
%                                March 2000                                   %
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
#include "blob.h"
#include "blob-private.h"
#include "constitute.h"
#include "exception.h"
#include "exception-private.h"
#include "image.h"
#include "image-private.h"
#include "list.h"
#include "magick.h"
#include "memory_.h"
#include "module.h"
#include "quantum-private.h"
#include "static.h"
#include "resource_.h"
#include "string_.h"
#include "string-private.h"
#include "utility.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e a d F D I m a g e                                                     %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  ReadFDImage retrieves an image via a file descriptor, decodes the image,
%  and returns it.  It allocates the memory necessary for the new Image
%  structure and returns a pointer to the new image.
%
%  The format of the ReadFDImage method is:
%
%      Image *ReadFDImage(const ImageInfo *image_info,ExceptionInfo *exception)
%
%  A description of each parameter follows:
%
%    o image_info: the image info.
%
%    o exception: return any errors or warnings in this structure.
%
*/
static Image *ReadFDImage(const ImageInfo *image_info,ExceptionInfo *exception)
{
  Image
    *image;

  ImageInfo
    *read_info;

  /*
    Open image file.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickCoreSignature);
  if (image_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
      image_info->filename);
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickCoreSignature);
  read_info=CloneImageInfo(image_info);
  read_info->file=fdopen(StringToLong(image_info->filename),"rb");
  if ((read_info->file ==  (FILE *) NULL) ||
      (IsGeometry(image_info->filename) == MagickFalse))
    {
      read_info=DestroyImageInfo(read_info);
      ThrowFileException(exception,BlobError,"UnableToOpenBlob",
        image_info->filename);
      return((Image *) NULL);
    }
  *read_info->magick='\0';
  image=ReadImage(read_info,exception);
  (void) fclose(read_info->file);
  read_info=DestroyImageInfo(read_info);
  if (image == (Image *) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),CoderError,
        "NoDataReturned","`%s'",image_info->filename);
      return((Image *) NULL);
    }
  return(GetFirstImageInList(image));
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e g i s t e r F D I m a g e                                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  RegisterFDImage() adds attributes for the FD image format to
%  the list of supported formats.  The attributes include the image format
%  tag, a method to read and/or write the format, whether the format
%  supports the saving of more than one frame to the same file or blob,
%  whether the format supports native in-memory I/O, and a brief
%  description of the format.
%
%  The format of the RegisterFDImage method is:
%
%      size_t RegisterFDImage(void)
%
*/
ModuleExport size_t RegisterFDImage(void)
{
  MagickInfo
    *entry;

  entry=AcquireMagickInfo("FD","FD","Read image from a file descriptor");
  entry->decoder=(DecodeImageHandler *) ReadFDImage;
  entry->flags|=CoderStealthFlag;
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U n r e g i s t e r F D I m a g e                                         %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  UnregisterFDImage() removes format registrations made by the FD module from
%  the list of supported formats.
%
%  The format of the UnregisterFDImage method is:
%
%      UnregisterFDImage(void)
%
*/
ModuleExport void UnregisterFDImage(void)
{
  (void) UnregisterMagickInfo("FD");
}
