#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }

int SeamCarver::GetEnergy(int row, int col) const {
  return (GetRowEnergy(row, col) + GetColEnergy(row, col));
}

int SeamCarver::GetRowEnergy(int row, int col) const {
  ImagePPM ip = GetImage();
  int r_row = 0;
  int g_row = 0;
  int b_row = 0;
  int delta_row = 0;
  if (row == 0) {
    r_row = ip.GetPixel(height_ - 1, col).GetRed() -
            ip.GetPixel(row + 1, col).GetRed();
    g_row = ip.GetPixel(height_ - 1, col).GetGreen() -
            ip.GetPixel(row + 1, col).GetGreen();
    b_row = ip.GetPixel(height_ - 1, col).GetBlue() -
            ip.GetPixel(row + 1, col).GetBlue();
  } else if (row == height_ - 1) {
    r_row = ip.GetPixel(row - 1, col).GetRed() - ip.GetPixel(0, col).GetRed();
    g_row =
        ip.GetPixel(row - 1, col).GetGreen() - ip.GetPixel(0, col).GetGreen();
    b_row = ip.GetPixel(row - 1, col).GetBlue() - ip.GetPixel(0, col).GetBlue();
  } else {
    r_row =
        ip.GetPixel(row - 1, col).GetRed() - ip.GetPixel(row + 1, col).GetRed();
    g_row = ip.GetPixel(row - 1, col).GetGreen() -
            ip.GetPixel(row + 1, col).GetGreen();
    b_row = ip.GetPixel(row - 1, col).GetBlue() -
            ip.GetPixel(row + 1, col).GetBlue();
  }
  delta_row = (int)((r_row * r_row) + (g_row * g_row) + (b_row * b_row));
  return delta_row;
}

int SeamCarver::GetColEnergy(int row, int col) const {
  ImagePPM ip = GetImage();
  int r_col = 0;
  int g_col = 0;
  int b_col = 0;
  int delta_col = 0;
  if (col == 0) {
    r_col = ip.GetPixel(row, width_ - 1).GetRed() -
            ip.GetPixel(row, col + 1).GetRed();
    g_col = ip.GetPixel(row, width_ - 1).GetGreen() -
            ip.GetPixel(row, col + 1).GetGreen();
    b_col = ip.GetPixel(row, width_ - 1).GetBlue() -
            ip.GetPixel(row, col + 1).GetBlue();
  } else if (col == width_ - 1) {
    r_col = ip.GetPixel(row, col - 1).GetRed() - ip.GetPixel(row, 0).GetRed();
    g_col =
        ip.GetPixel(row, col - 1).GetGreen() - ip.GetPixel(row, 0).GetGreen();
    b_col = ip.GetPixel(row, col - 1).GetBlue() - ip.GetPixel(row, 0).GetBlue();
  } else {
    r_col =
        ip.GetPixel(row, col - 1).GetRed() - ip.GetPixel(row, col + 1).GetRed();
    g_col = ip.GetPixel(row, col - 1).GetGreen() -
            ip.GetPixel(row, col + 1).GetGreen();
    b_col = ip.GetPixel(row, col - 1).GetBlue() -
            ip.GetPixel(row, col + 1).GetBlue();
  }
  delta_col = (int)((r_col * r_col) + (g_col * g_col) + (b_col * b_col));
  return delta_col;
}

int** SeamCarver::EnergyArr() const {
  ImagePPM image = GetImage();
  int** to_return = new int*[width_];
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_; col++) {
      to_return[row][col] = GetEnergy(row, col);
    }
  }
  return to_return;
  for (int i = 0; i < height_; i++) {
    delete[] to_return[i];
  }
  delete[] to_return;
  to_return = nullptr;
}

int** SeamCarver::FindMinPath() const {
  int** energy_arr = EnergyArr();
  for (int row = height_ - 2; row >= 0; row--) {
    for (int col = 0; col < width_; col++) {
      int best = 0;
      if (col == 0) {
        int bottom = energy_arr[row + 1][col];
        int botright = energy_arr[row + 1][col + 1];
        best = std::min(bottom, botright);
      } else if (col == width_ - 1) {
        int bottom = energy_arr[row + 1][col];
        int botleft = energy_arr[row + 1][col - 1];
        best = std::min(botleft, bottom);
      } else {
        int bottom = energy_arr[row + 1][col];
        int botleft = energy_arr[row + 1][col - 1];
        int botright = energy_arr[row + 1][col + 1];
        best = Min(botleft, bottom, botright);
      }
      energy_arr[row][col] = best + energy_arr[row][col];
    }
  }
  return energy_arr;
  for (int i = 0; i < height_; i++) {
    delete[] energy_arr[i];
  }
  delete[] energy_arr;
  energy_arr = nullptr;
}

void SeamCarver::VerticalSeamArray(int** input) const {
  for (int row = height_ - 2; row >= 0; row--) {
    for (int col = 0; col < width_; col++) {
      if (col == 0) {
        if (input[row + 1][col] <= input[row + 1][col + 1]) {
          input[row][col] = input[row][col] + input[row + 1][col];
        } else {
          input[row][col] = input[row][col] + input[row + 1][col + 1];
        }
      } else if (col == width_ - 1) {
        if (input[row + 1][col] <= input[row + 1][col - 1]) {
          input[row][col] += input[row + 1][col];
        } else {
          input[row][col] += input[row + 1][col - 1];
        }
      } else {
        if (input[row + 1][col] <= input[row + 1][col + 1] &&
            input[row + 1][col] <= input[row + 1][col - 1]) {
          input[row][col] += input[row + 1][col];
        } else if (input[row + 1][col - 1] <= input[row + 1][col + 1] &&
                   input[row + 1][col - 1] < input[row + 1][col]) {
          input[row][col] += input[row + 1][col - 1];
        } else {
          input[row][col] += input[row + 1][col + 1];
        }
      }
    }
  }
}

void SeamCarver::HorizontalSeamArray(int** input) const {
  for (int col = width_ - 2; col >= 0; col--) {
    for (int row = 0; row < height_; row++) {
      if (row == 0) {
        if (input[row][col + 1] <= input[row + 1][col + 1]) {
          input[row][col] += input[row][col + 1];
        } else {
          input[row][col] += input[row + 1][col + 1];
        }
      } else if (row == height_ - 1) {
        if (input[row][col + 1] <= input[row - 1][col + 1]) {
          input[row][col] += input[row][col + 1];
        } else {
          input[row][col] += input[row - 1][col + 1];
        }
      } else {
        if (input[row][col + 1] <= input[row + 1][col + 1] &&
            input[row][col + 1] <= input[row - 1][col + 1]) {
          input[row][col] += input[row][col + 1];
        } else if (input[row - 1][col + 1] <= input[row][col + 1] &&
                   input[row - 1][col + 1] < input[row + 1][col + 1]) {
          input[row][col] += input[row - 1][col + 1];
        } else {
          input[row][col] += input[row + 1][col + 1];
        }
      }
    }
  }
}

void SeamCarver::CalculateVertSeam(int** input, int* seam) const {
  int smallest = 0;
  for (int col = 1; col < width_; col++) {
    if (input[0][col] < input[0][smallest]) {
      smallest = col;
      seam[0] = smallest;
    }
  }
  for (int row = 1; row < height_; row++) {
    if (smallest == 0) {
      if (input[row][smallest] <= input[row][smallest + 1]) {
        seam[row] = smallest;
      } else {
        seam[row] = smallest + 1;
        smallest = smallest + 1;
      }
    } else if (smallest == width_ - 1) {
      if (input[row][smallest] <= input[row][smallest - 1]) {
        seam[row] = smallest;
      } else {
        seam[row] = smallest - 1;
        smallest = smallest - 1;
      }
    } else {
      if (input[row][smallest] <= input[row][smallest + 1] &&
          input[row][smallest] <= input[row][smallest - 1]) {
        seam[row] = smallest;
      } else if (input[row][smallest - 1] <= input[row][smallest + 1] &&
                 input[row][smallest - 1] < input[row][smallest]) {
        seam[row] = smallest - 1;
        smallest = smallest - 1;
      } else {
        seam[row] = smallest + 1;
        smallest = smallest + 1;
      }
    }
  }
}

void SeamCarver::CalculateHoriSeam(int** input, int* seam) const {
  int smallest = 0;
  for (int row = 1; row < height_; row++) {
    if (input[row][0] < input[smallest][0]) {
      smallest = row;
      seam[0] = smallest;
    }
  }
  for (int col = 1; col < width_; col++) {
    if (smallest == 0) {
      if (input[smallest][col] <= input[smallest + 1][col]) {
        seam[col] = smallest;
      } else {
        seam[col] = smallest + 1;
        smallest += 1;
      }
    } else if (smallest == height_ - 1) {
      if (input[smallest][col] <= input[smallest - 1][col]) {
        seam[col] = smallest;
      } else {
        seam[col] = smallest - 1;
        smallest -= 1;
      }
    } else {
      if (input[smallest][col] <= input[smallest + 1][col] &&
          input[smallest][col] <= input[smallest - 1][col]) {
        seam[col] = smallest;
      } else if (input[smallest - 1][col] <= input[smallest + 1][col] &&
                 input[smallest - 1][col] < input[smallest][col]) {
        seam[col] = smallest - 1;
        smallest -= 1;
      } else {
        seam[col] = smallest + 1;
        smallest += 1;
      }
    }
  }
}

int SeamCarver::Min(int a, int b, int c) const {
  int to_return = 0;
  if (a <= b && a <= c) {
    to_return = a;
  } else if (b <= c && b <= a) {
    to_return = b;
  } else if (c < b && c < a) {
    to_return = c;
  }
  return to_return;
}

int* SeamCarver::GetHorizontalSeam() const {
  int* seam = new int[height_];
  int** min_path = EnergyArr();
  HorizontalSeamArray(min_path);
  CalculateHoriSeam(min_path, seam);
  for (int i = 0; i < height_; i++) {
    delete[] min_path[i];
  }
  delete[] min_path;
  min_path = nullptr;
  return seam;
}

int* SeamCarver::GetVerticalSeam() const {
  int* seam = new int[height_];
  int** to_return = EnergyArr();
  VerticalSeamArray(to_return);
  CalculateVertSeam(to_return, seam);
  for (int i = 0; i < height_; i++) {
    delete[] to_return[i];
  }
  delete[] to_return;
  to_return = nullptr;
  return seam;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* seam = GetHorizontalSeam();
  image_.RemoveHorizontalCarve(seam);
  delete[] seam;
  seam = nullptr;
  height_ -= 1;
}
void SeamCarver::RemoveVerticalSeam() {
  int* seam = GetVerticalSeam();
  image_.RemoveVerticalCarve(seam);
  delete[] seam;
  seam = nullptr;
  width_ -= 1;
}