object FormLineDetails: TFormLineDetails
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Line tool'
  ClientHeight = 194
  ClientWidth = 246
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  PopupMode = pmExplicit
  Position = poDesigned
  ScreenSnap = True
  SnapBuffer = 20
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnKeyUp = FormKeyUp
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 4
    Top = 70
    Width = 126
    Height = 64
    Caption = 'Brush + line fx'
    TabOrder = 0
    object btnTaper2: TSpeedButton
      Left = 8
      Top = 40
      Width = 110
      Height = 16
      AllowAllUp = True
      GroupIndex = 21
      Caption = '+ taper from midpoint'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnTaper2MouseEnter
      OnMouseLeave = btnTaperInMouseLeave
    end
    object btnTaperIn: TSpeedButton
      Left = 8
      Top = 18
      Width = 52
      Height = 16
      AllowAllUp = True
      GroupIndex = 22
      Caption = 'Taper in'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnTaperInClick
      OnMouseEnter = btnTaperInMouseEnter
      OnMouseLeave = btnTaperInMouseLeave
    end
    object btnTaperOut: TSpeedButton
      Left = 66
      Top = 18
      Width = 52
      Height = 16
      AllowAllUp = True
      GroupIndex = 23
      Caption = 'Taper out'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnTaperInClick
      OnMouseEnter = btnTaperOutMouseEnter
      OnMouseLeave = btnTaperInMouseLeave
    end
  end
  object GroupBox2: TGroupBox
    Left = 4
    Top = 4
    Width = 126
    Height = 64
    Caption = 'Line style'
    TabOrder = 1
    object btnDots: TSpeedButton
      Left = 8
      Top = 18
      Width = 52
      Height = 16
      AllowAllUp = True
      GroupIndex = 10
      Caption = 'Dots'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnDotsMouseEnter
      OnMouseLeave = btnTaperInMouseLeave
    end
    object btnDashes: TSpeedButton
      Left = 66
      Top = 18
      Width = 52
      Height = 16
      AllowAllUp = True
      GroupIndex = 10
      Caption = 'Dashes'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnDashesMouseEnter
      OnMouseLeave = btnTaperInMouseLeave
    end
    object Label1: TLabel
      Left = 10
      Top = 40
      Width = 27
      Height = 11
      Caption = 'Length'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = Label1MouseEnter
      OnMouseLeave = btnTaperInMouseLeave
    end
    object LabelDashLen: TLabel
      Left = 108
      Top = 40
      Width = 10
      Height = 11
      Caption = 'LE'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseLeave = btnTaperInMouseLeave
    end
    object TrkDash: TTrackBar
      Left = 39
      Top = 35
      Width = 65
      Height = 20
      Max = 19
      Min = 1
      PageSize = 1
      Position = 1
      TabOrder = 0
      TabStop = False
      ThumbLength = 7
      TickMarks = tmTopLeft
      OnChange = TrkDashChange
    end
  end
  object GroupBox3: TGroupBox
    Left = 136
    Top = 4
    Width = 107
    Height = 130
    Caption = 'Scrollwheel roster'
    TabOrder = 2
    object btnRosterLine: TSpeedButton
      Left = 8
      Top = 84
      Width = 64
      Height = 16
      AllowAllUp = True
      GroupIndex = 1
      Down = True
      Caption = 'Line'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnRosterLineClick
      OnMouseDown = btnRosterLineMouseDown
      OnMouseLeave = btnTaperInMouseLeave
    end
    object btnRosterCurve: TSpeedButton
      Left = 8
      Top = 62
      Width = 64
      Height = 16
      AllowAllUp = True
      GroupIndex = 2
      Down = True
      Caption = 'Curve'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnRosterLineClick
      OnMouseDown = btnRosterLineMouseDown
      OnMouseLeave = btnTaperInMouseLeave
    end
    object btnRosterKnee: TSpeedButton
      Left = 8
      Top = 40
      Width = 64
      Height = 16
      AllowAllUp = True
      GroupIndex = 3
      Down = True
      Caption = 'Knee angle'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnRosterLineClick
      OnMouseDown = btnRosterLineMouseDown
      OnMouseLeave = btnTaperInMouseLeave
    end
    object btnRosterAngle: TSpeedButton
      Left = 8
      Top = 18
      Width = 64
      Height = 16
      AllowAllUp = True
      GroupIndex = 4
      Down = True
      Caption = 'Right angle'
      Flat = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnRosterLineClick
      OnMouseDown = btnRosterLineMouseDown
      OnMouseLeave = btnTaperInMouseLeave
    end
    object btnResetLine: TSpeedButton
      Left = 8
      Top = 106
      Width = 22
      Height = 16
      AllowAllUp = True
      GroupIndex = 30
      OnMouseLeave = btnTaperInMouseLeave
    end
    object Label2: TLabel
      Left = 36
      Top = 103
      Width = 33
      Height = 11
      Caption = 'reset on'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseLeave = btnTaperInMouseLeave
    end
    object Label3: TLabel
      Left = 36
      Top = 112
      Width = 35
      Height = 11
      Caption = 'new line'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseLeave = btnTaperInMouseLeave
    end
    object LineToolIndicator: TLabel
      Left = 77
      Top = 84
      Width = 23
      Height = 15
      Caption = '`z'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Wingdings 3'
      Font.Style = []
      ParentFont = False
      OnMouseLeave = btnTaperInMouseLeave
    end
  end
  object CheckEnableBrush: TCheckBox
    Left = 108
    Top = 68
    Width = 14
    Height = 17
    Checked = True
    State = cbChecked
    TabOrder = 3
    OnMouseEnter = CheckEnableBrushMouseEnter
    OnMouseLeave = btnTaperInMouseLeave
  end
  object GroupBox4: TGroupBox
    Left = 4
    Top = 136
    Width = 126
    Height = 55
    Caption = 'Mode'
    TabOrder = 4
    object btnSmear: TSpeedButton
      Left = 10
      Top = 33
      Width = 50
      Height = 16
      AllowAllUp = True
      GroupIndex = 40
      Caption = 'Coa&t'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnSmearMouseEnter
      OnMouseLeave = btnTaperInMouseLeave
    end
    object btnQuick: TSpeedButton
      Left = 10
      Top = 13
      Width = 108
      Height = 16
      AllowAllUp = True
      GroupIndex = 42
      Caption = 'Quic&k multiline'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseLeave = btnTaperInMouseLeave
    end
    object btnMove: TSpeedButton
      Left = 68
      Top = 33
      Width = 50
      Height = 16
      AllowAllUp = True
      GroupIndex = 41
      Caption = 'Mo&ve'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseEnter = btnMoveMouseEnter
      OnMouseLeave = btnTaperInMouseLeave
    end
  end
  object GroupBox5: TGroupBox
    Left = 136
    Top = 136
    Width = 107
    Height = 55
    Caption = 'Adjust x0y0 (wasd)'
    TabOrder = 5
    object SpeedButton3: TSpeedButton
      Left = 8
      Top = 18
      Width = 84
      Height = 16
      AllowAllUp = True
      GroupIndex = 10
      Caption = '&Center offset'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton3Click
      OnMouseEnter = SpeedButton3MouseEnter
      OnMouseLeave = btnTaperInMouseLeave
    end
    object Label4: TLabel
      Left = 30
      Top = 37
      Width = 64
      Height = 11
      Caption = 'autoreset offset'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnMouseLeave = btnTaperInMouseLeave
    end
    object CheckResetLineNudge: TCheckBox
      Left = 8
      Top = 35
      Width = 14
      Height = 17
      Checked = True
      State = cbChecked
      TabOrder = 0
      OnMouseEnter = CheckResetLineNudgeMouseEnter
      OnMouseLeave = btnTaperInMouseLeave
    end
  end
end
