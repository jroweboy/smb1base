object FormTileBucketExtras: TFormTileBucketExtras
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Tile Bucket Extras'
  ClientHeight = 105
  ClientWidth = 237
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDesigned
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  PixelsPerInch = 96
  TextHeight = 13
  object CheckAlignToSel: TCheckBox
    Left = 5
    Top = 5
    Width = 222
    Height = 17
    TabStop = False
    Caption = 'Align multi-tile placement to map selection'
    TabOrder = 0
    OnMouseEnter = CheckAlignToSelMouseEnter
  end
  object CheckSubpaletteAware: TCheckBox
    Left = 5
    Top = 22
    Width = 128
    Height = 17
    TabStop = False
    Caption = 'Subpalette Aware'
    Checked = True
    State = cbChecked
    TabOrder = 1
    OnMouseEnter = CheckSubpaletteAwareMouseEnter
  end
  object GroupBox6: TGroupBox
    Left = 2
    Top = 45
    Width = 234
    Height = 60
    Caption = 'Capped flood reach (beta)'
    TabOrder = 2
    object btnSetMaxReach: TSpeedButton
      Left = 5
      Top = 18
      Width = 48
      Height = 18
      AllowAllUp = True
      GroupIndex = 1
      Caption = 'Turn on'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnSetMaxReachMouseEnter
    end
    object TrkReach: TTrackBar
      Left = 58
      Top = 15
      Width = 165
      Height = 20
      Max = 32
      Min = 2
      PageSize = 1
      Position = 8
      TabOrder = 0
      TabStop = False
      ThumbLength = 10
      TickMarks = tmTopLeft
      OnChange = TrkReachChange
    end
    object chkAutoCustom: TCheckBox
      Left = 5
      Top = 40
      Width = 222
      Height = 17
      TabStop = False
      Caption = 'Auto-use custom flood direction (shift+f6)'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnMouseEnter = chkAutoCustomMouseEnter
    end
  end
end
