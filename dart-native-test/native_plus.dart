import 'dart-ext:native_plus';
int nativePlus(int a, int b) native "NativePlus";
void main() {
  print(nativePlus(3,5));
}