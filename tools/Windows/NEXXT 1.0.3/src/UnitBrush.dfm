object FormBrush: TFormBrush
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Brush mask'
  ClientHeight = 80
  ClientWidth = 82
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PopupMode = pmExplicit
  Position = poDesigned
  ScreenSnap = True
  SnapBuffer = 20
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object PaintBoxMask: TPaintBox
    Left = 8
    Top = 8
    Width = 64
    Height = 64
    OnMouseDown = PaintBoxMaskMouseDown
    OnMouseMove = PaintBoxMaskMouseMove
    OnPaint = PaintBoxMaskPaint
  end
  object BrushmaskTimer: TTimer
    Enabled = False
    Interval = 50
    OnTimer = BrushmaskTimerTimer
    Left = 30
    Top = 37
  end
end
