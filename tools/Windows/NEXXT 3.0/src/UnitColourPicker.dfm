object FormColourPicker: TFormColourPicker
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Colour Rose'
  ClientHeight = 288
  ClientWidth = 288
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  OnMouseEnter = FormMouseEnter
  OnMouseLeave = FormMouseLeave
  OnMouseWheel = FormMouseWheel
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 0
    Top = 0
    Width = 288
    Height = 288
    OnMouseDown = Image1MouseDown
    OnMouseLeave = Image1MouseLeave
    OnMouseMove = Image1MouseMove
  end
  object TimerPassePartout: TTimer
    Enabled = False
    Interval = 20
    OnTimer = TimerPassePartoutTimer
    Left = 128
    Top = 136
  end
  object TimerHighlightSubpal: TTimer
    Enabled = False
    Interval = 17
    OnTimer = TimerHighlightSubpalTimer
    Left = 136
    Top = 144
  end
  object TimerRefresh: TTimer
    Enabled = False
    Interval = 67
    OnTimer = TimerRefreshTimer
    Left = 144
    Top = 152
  end
end
