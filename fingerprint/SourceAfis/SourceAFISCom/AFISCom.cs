using System;
using SourceAFIS.Extraction;
using SourceAFIS.Templates;

namespace SourceAFISCom
{
  public class AFISCom : IAFISCom
  {
    Extractor Extractor = new Extractor();
    public AFISCom()
    {
      
    }

    public byte[] extract(byte[] abytImg, int lngDpi)
    {
      if (abytImg.Length <= 8)
        throw new ApplicationException("Raw image array is too short.");

      int height = BitConverter.ToInt32(abytImg, 0);
      int width = BitConverter.ToInt32(abytImg, 4);

      if (height <= 0 || width <= 0)
        throw new ApplicationException("Invalid image dimensions in raw image array.");
      if (8 + width * height != abytImg.Length)
        throw new ApplicationException("Incorrect length of raw image array.");

      byte[,] unpacked = new byte[height, width];
      for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
          unpacked[y, x] = abytImg[8 + y * width + x];

      TemplateBuilder builder =  Extractor.Extract(unpacked, lngDpi);
      return builder != null ? new CompactFormat().Export(builder) : null;
    }
  }
}
